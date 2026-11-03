#include "cli/ConsoleApp.h"

#include <istream>
#include <ostream>
#include <utility>

#include "cli/CommandParser.h"
#include "core/Game.h"
#include "core/LevelLoader.h"
#include "util/ConsoleColor.h"
#include "util/StringUtil.h"

namespace zuma::cli {

ConsoleApp::ConsoleApp(std::istream& in, std::ostream& out, bool useColor, bool clearScreen,
                       const std::vector<std::string>& levelPaths)
    : in_(in), out_(out), levelPaths_(levelPaths) {
    ViewOptions options;
    options.useColor = useColor;
    options.clearScreen = clearScreen;
    view_ = std::make_unique<ConsoleView>(colors_, options, out_);
}

void ConsoleApp::RenderStartupMessages() {
    if (startupWarnings_.empty() && startupErrors_.empty()) {
        return;
    }
    view_->RenderWarnings(startupWarnings_);
    for (const std::string& error : startupErrors_) {
        view_->RenderError(error);
    }
    // 只在第一帧显示一次，避免每帧重复
    startupWarnings_.clear();
    startupErrors_.clear();
}

bool ConsoleApp::DiscoverLevels() {
    startupWarnings_.clear();
    startupErrors_.clear();

    if (!levelPaths_.empty()) {
        for (const std::string& path : levelPaths_) {
            std::string error;
            std::optional<core::LevelLoadReport> report = core::LevelLoader::LoadFile(path, error);
            if (!report) {
                startupErrors_.push_back(error);
                continue;
            }
            startupWarnings_.insert(startupWarnings_.end(), report->warnings.begin(),
                                    report->warnings.end());
            levels_.push_back(std::move(report->level));
        }
    } else {
        // 依次尝试若干候选目录（保证从构建目录或项目根目录运行都能找到关卡）
        const std::vector<std::string> candidates = {"levels", "../levels", "../../levels",
                                                     "../../../levels"};
        for (const std::string& directory : candidates) {
            std::vector<std::string> loadErrors;
            const std::vector<core::Level> found =
                core::LevelLoader::LoadDirectory(directory, loadErrors);
            if (!found.empty()) {
                levels_ = found;
                startupErrors_ = loadErrors;
                break;
            }
        }
    }

    if (levels_.empty()) {
        view_->RenderWarnings(startupWarnings_);
        for (const std::string& error : startupErrors_) {
            view_->RenderError(error);
        }
        view_->RenderError("没有找到任何可用关卡。请在项目根目录（或构建目录）下运行，"
                           "或把关卡文件路径作为参数传入，例如：zuma levels/level1.txt");
        return false;
    }
    return true;
}

std::optional<std::string> ConsoleApp::ReadLine() {
    std::string line;
    if (!std::getline(in_, line)) {
        return std::nullopt;
    }
    // 非交互输入（管道/重定向）时输入不会被回显，补一个换行让输出与提示分行。
    if (!util::StdinIsTerminal()) {
        out_ << "\n";
    }
    return line;
}

std::optional<core::Level> ConsoleApp::ChooseLevel() {
    if (levels_.empty()) {
        return std::nullopt;
    }
    if (levels_.size() == 1) {
        return levels_.front();
    }

    std::string message;  // 上一轮的错误提示，在下一帧顶部显示
    while (true) {
        view_->BeginFrame();
        RenderStartupMessages();
        if (!message.empty()) {
            view_->RenderError(message);
        }
        view_->RenderLevelMenu(levels_);
        view_->Prompt("  请选择关卡 [1-" + std::to_string(levels_.size()) + "]（Q 退出）： ");
        const std::optional<std::string> line = ReadLine();
        if (!line) {
            out_ << "\n";
            return std::nullopt;
        }

        message.clear();
        const std::string head = util::ToUpperAscii(util::Trim(*line));
        if (head == "Q" || head == "QUIT" || head == "EXIT") {
            return std::nullopt;
        }

        std::size_t choice = 0;
        if (!util::ParseUnsigned(head, choice) || choice < 1 || choice > levels_.size()) {
            message = "请输入 1.." + std::to_string(levels_.size()) + " 之间的关卡编号";
            continue;
        }
        return levels_[choice - 1];
    }
}

ConsoleApp::PostGameAction ConsoleApp::PlayOneGame(const core::Level& level) {
    core::Game game(level, colors_);

    view_->BeginFrame();
    RenderStartupMessages();
    view_->RenderWarnings(game.Warnings());
    view_->Render(game);

    while (game.Status() == core::GameStatus::Playing) {
        view_->Prompt();
        const std::optional<std::string> line = ReadLine();
        if (!line) {
            out_ << "\n";
            return PostGameAction::Exit;
        }

        const Command command = CommandParser::Parse(*line);
        // 每条指令处理完都重开一帧：屏幕上只保留「本回合发生了什么 + 当前状态」。
        view_->BeginFrame();

        switch (command.type) {
            case CommandType::Invalid:
                view_->RenderError(command.error);
                view_->Render(game);
                break;

            case CommandType::Show:
                view_->Render(game);
                break;

            case CommandType::Help:
                view_->RenderHelp();
                break;

            case CommandType::Hint:
                view_->RenderInfo("提示功能为预留扩展接口，尚未启用（见架构文档第 9 节）。");
                view_->Render(game);
                break;

            case CommandType::Undo:
                if (game.Undo()) {
                    view_->RenderInfo("已撤销上一回合。");
                } else {
                    view_->RenderError("没有可撤销的回合。");
                }
                view_->Render(game);
                break;

            case CommandType::Quit:
                game.Quit();
                view_->RenderInfo("已退出本局。");
                break;

            case CommandType::Fire:
            case CommandType::Drop: {
                const core::TurnResult result =
                    command.type == CommandType::Fire ? game.Fire(command.arg) : game.Drop();
                if (!result.ok) {
                    view_->RenderError(result.message);
                } else {
                    view_->RenderTurnResult(result, game);
                    out_ << "\n";
                }
                view_->Render(game);
                break;
            }
        }

        if (game.Status() == core::GameStatus::Quit) {
            return PostGameAction::Quit;
        }
    }

    return AskPostGameAction(game);
}

ConsoleApp::PostGameAction ConsoleApp::AskPostGameAction(const core::Game& game) {
    std::string message;
    bool firstFrame = true;  // 首帧沿用上一帧的棋盘与回合日志，不重复清屏

    while (true) {
        if (firstFrame) {
            view_->RenderOutcome(game);
        } else {
            view_->BeginFrame();
            view_->Render(game);
            view_->RenderOutcome(game);
        }
        if (!message.empty()) {
            view_->RenderError(message);
        }

        view_->Prompt("  [R] 重玩本关   [N] 换关卡   [Q] 退出 > ");
        const std::optional<std::string> line = ReadLine();
        if (!line) {
            out_ << "\n";
            return PostGameAction::Exit;
        }
        firstFrame = false;

        const std::string head = util::ToUpperAscii(util::Trim(*line));
        if (head == "R") {
            return PostGameAction::Replay;
        }
        if (head == "N") {
            return PostGameAction::ChangeLevel;
        }
        if (head == "Q" || head == "QUIT" || head == "EXIT") {
            return PostGameAction::Quit;
        }
        message = "请输入 R（重玩本关）/ N（换关卡）/ Q（退出）";
    }
}

int ConsoleApp::Run() {
    if (!DiscoverLevels()) {
        return 1;
    }

    bool keepRunning = true;
    while (keepRunning) {
        const std::optional<core::Level> level = ChooseLevel();
        if (!level) {
            break;
        }

        bool replay = true;
        while (replay) {
            switch (PlayOneGame(*level)) {
                case PostGameAction::Replay:
                    break;  // 重玩本关
                case PostGameAction::ChangeLevel:
                    replay = false;
                    break;
                case PostGameAction::Quit:
                case PostGameAction::Exit:
                    replay = false;
                    keepRunning = false;
                    break;
            }
        }
    }

    return 0;
}

}  // namespace zuma::cli
