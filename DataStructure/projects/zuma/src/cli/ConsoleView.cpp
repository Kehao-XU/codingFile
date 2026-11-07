#include "cli/ConsoleView.h"

#include <iomanip>
#include <ostream>

namespace zuma::cli {

ConsoleView::ConsoleView(const core::ColorRegistry& colors, ViewOptions options, std::ostream& out)
    : colors_(colors), options_(options), out_(out) {}

void ConsoleView::BeginFrame() const {
    if (!options_.clearScreen) {
        return;
    }
    // 2J 清屏、3J 清滚动缓冲、H 光标归位。不支持的终端会忽略未知参数，不会出错。
    out_ << "\x1b[2J\x1b[3J\x1b[H";
}

std::string ConsoleView::BallText(char color) const {
    std::string cell = "[";
    cell += color;
    cell += "]";
    if (!options_.useColor) {
        return cell;
    }
    const std::string ansi = colors_.Get(color).ansiFg;
    if (ansi.empty()) {
        return cell;
    }
    return ansi + cell + "\x1b[0m";
}

std::string ConsoleView::FeedText(const std::deque<core::Ball>& feed) const {
    std::string text;
    const std::size_t shown = feed.size() < options_.feedDisplayLimit ? feed.size()
                                                                      : options_.feedDisplayLimit;
    for (std::size_t index = 0; index < shown; ++index) {
        if (index != 0) {
            text += ' ';
        }
        text += feed[index].color;
    }
    if (feed.size() > shown) {
        text += " ...(还有 " + std::to_string(feed.size() - shown) + " 个)";
    }
    return text;
}

const char* ConsoleView::StatusName(core::GameStatus status) {
    switch (status) {
        case core::GameStatus::Playing:
            return "进行中";
        case core::GameStatus::Won:
            return "胜利";
        case core::GameStatus::Lost:
            return "失败";
        case core::GameStatus::Quit:
            return "已退出";
    }
    return "未知";
}

void ConsoleView::Render(const core::Game& game) const {
    const core::GameViewData& view = game.View();
    const std::size_t trackLength = view.track == nullptr ? 0 : view.track->size();

    out_ << "=== 祖玛彩球  |  关卡：" << view.levelName << "  ===\n\n";

    if (trackLength == 0) {
        out_ << "  轨道：<空>\n";
    } else {
        out_ << "  位置：";
        for (std::size_t index = 1; index <= trackLength; ++index) {
            out_ << std::setw(4) << index;
        }
        out_ << "\n  轨道：";
        for (const core::Ball& ball : *view.track) {
            out_ << BallText(ball.color) << ' ';
        }
        out_ << "  <- 入口端\n";
    }

    out_ << "  长度：" << trackLength << " / " << view.maxLength << "\n";

    out_ << "  等待区";
    if (view.feed == nullptr || view.feed->empty()) {
        out_ << "：<空>\n";
    } else {
        out_ << "（" << view.feed->size() << "）：" << FeedText(*view.feed) << "\n";
    }

    out_ << "  当前球：" << BallText(view.currentBall) << "    下一个："
         << BallText(view.nextBall) << "\n";
    out_ << "  得分：" << view.score << "    回合：" << view.turn
         << "    最大连锁：" << view.maxCombo << "\n";
    out_ << "  状态：" << StatusName(view.status);
    if (view.canUndo) {
        out_ << "    （可输入 UNDO 撤销上一回合）";
    }
    out_ << "\n\n";
}

void ConsoleView::RenderTurnResult(const core::TurnResult& result, const core::Game& game) const {
    const core::GameViewData& view = game.View();

    out_ << "[回合 " << view.turn << "] ";
    if (result.isDrop) {
        out_ << "丢弃 " << BallText(result.ball) << "（丢弃不触发消除）\n";
    } else {
        out_ << "发射 " << BallText(result.ball) << " → 位置 " << result.position << "\n";

        if (result.rounds.empty()) {
            out_ << "  - 未形成消除\n";
        }
        for (const core::EliminationRound& round : result.rounds) {
            if (round.round == 1) {
                out_ << "  ✓ 普通消除：" << round.count << " 个 " << BallText(round.color) << "（+"
                     << round.gainedScore << " 分）\n";
            } else {
                out_ << "  ✓ 连锁消除 #" << round.round << "：" << round.count << " 个 "
                     << BallText(round.color) << "（+" << round.gainedScore << " 分）\n";
            }
        }
        if (!result.rounds.empty()) {
            out_ << "  ✓ 本次共消除 " << result.eliminated() << " 个球，合计 +"
                 << result.scoreGained << " 分\n";
        }
    }

    if (result.fed) {
        const std::size_t remaining = view.feed == nullptr ? 0 : view.feed->size();
        out_ << "  → 等待区入球：" << BallText(result.fedColor) << "（剩余 " << remaining << "）\n";
    } else {
        out_ << "  → 等待区已空，无新球入轨\n";
    }

    out_ << "  → 手持更新：当前球 " << BallText(view.currentBall) << "，下一个 "
         << BallText(view.nextBall) << "\n";
}

void ConsoleView::RenderOutcome(const core::Game& game) const {
    const core::GameViewData& view = game.View();
    const std::size_t trackLength = view.track == nullptr ? 0 : view.track->size();

    out_ << "\n==================== 本局结束 ====================\n";
    switch (view.status) {
        case core::GameStatus::Won:
            out_ << "  ★ 恭喜通关！轨道与等待区已全部清空。\n";
            break;
        case core::GameStatus::Lost:
            out_ << "  ✗ 游戏失败：轨道长度 " << trackLength << " 超过上限 " << view.maxLength
                 << "。\n";
            break;
        case core::GameStatus::Quit:
            out_ << "  已退出本局（不计胜负）。\n";
            break;
        case core::GameStatus::Playing:
            out_ << "  本局仍在进行中。\n";
            break;
    }
    out_ << "  得分：" << view.score << "    回合数：" << view.turn
         << "    最大连锁：" << view.maxCombo << "    通关关卡：" << view.levelName << "\n";
    out_ << "==================================================\n";
}

void ConsoleView::RenderHelp() const {
    out_ << "---------------- 命令说明 ----------------\n"
         << "  FIRE <n> / F <n>   把当前球插入到位置 n 之前\n"
         << "                     n 的范围：1 .. 轨道长度+1（n=长度+1 表示放到入口端）\n"
         << "  DROP / D           丢弃当前球（不触发消除，但仍算一个有效回合）\n"
         << "  SHOW / S           重新显示当前状态\n"
         << "  UNDO / U           撤销上一回合\n"
         << "  HINT               提示（预留扩展接口，尚未启用）\n"
         << "  HELP / H / ?       显示本帮助\n"
         << "  QUIT / Q / EXIT    退出本局\n"
         << "------------------------------------------\n"
         << "  规则要点：\n"
         << "   1) 插入后若形成 3 个及以上同色连续球，立即消除；\n"
         << "      消除后断点两侧若重新合并成 3 连以上，会继续连锁消除。\n"
         << "   2) 每个有效回合结束后，等待区会有一个球进入轨道（等待区为空则不加）。\n"
         << "   3) 轨道长度超过关卡上限 -> 失败；轨道与等待区同时清空 -> 胜利。\n"
         << "   4) 弹药按关卡 AMMO 规律无限循环生成，不存在弹药耗尽。\n"
         << "   5) 非法输入不消耗彩球、不增长轨道、不推进回合。\n"
         << "------------------------------------------\n";
}

void ConsoleView::RenderError(const std::string& message) const {
    out_ << "  ✗ " << message << "\n";
}

void ConsoleView::RenderInfo(const std::string& message) const {
    out_ << "  • " << message << "\n";
}

void ConsoleView::RenderWarnings(const std::vector<std::string>& warnings) const {
    for (const std::string& warning : warnings) {
        out_ << "  ⚠ " << warning << "\n";
    }
}

void ConsoleView::RenderLevelMenu(const std::vector<core::Level>& levels) const {
    out_ << "================ 可用关卡 ================\n";
    for (std::size_t index = 0; index < levels.size(); ++index) {
        out_ << "  " << (index + 1) << ") " << levels[index].name << "   [" << levels[index].sourcePath
             << "]\n"
             << "      初始球链 " << levels[index].active.size() << " 个，等待区 "
             << levels[index].feed.size() << " 个，上限 " << levels[index].maxLength << "\n";
    }
    out_ << "==========================================\n";
}

void ConsoleView::Prompt(const std::string& text) const {
    out_ << text;
    out_.flush();
}

}  // namespace zuma::cli
