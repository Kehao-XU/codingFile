#include <cstddef>
#include <sstream>
#include <string>

#include "cli/ConsoleView.h"
#include "core/ColorRegistry.h"
#include "core/Game.h"
#include "test_harness.h"

namespace {

// BeginFrame 在开启清屏时应输出的 ANSI 序列：2J 清屏 + 3J 清滚动缓冲 + H 归位。
const char* kClearSequence = "\x1b[2J\x1b[3J\x1b[H";

zuma::core::Level MakeLevel(const std::string& active = "RRB", const std::string& feed = "GY",
                            const std::string& ammo = "RG", std::size_t maxLength = 8) {
    zuma::core::Level level;
    level.name = "视图测试关";
    level.sourcePath = "view-test.txt";
    level.active = active;
    level.feed = feed;
    level.ammo = ammo;
    level.maxLength = maxLength;
    return level;
}

std::size_t CountOccurrences(const std::string& text, const std::string& needle) {
    if (needle.empty()) {
        return 0;
    }
    std::size_t count = 0;
    std::size_t position = text.find(needle, 0);
    while (position != std::string::npos) {
        ++count;
        position = text.find(needle, position + needle.size());
    }
    return count;
}

}  // namespace

void RunConsoleViewTests() {
    using zuma::cli::ConsoleView;
    using zuma::cli::ViewOptions;
    using zuma::core::ColorRegistry;
    using zuma::core::Game;

    const ColorRegistry colors;
    const std::size_t clearLength = std::string(kClearSequence).size();

    CASE("View: 清屏开关决定是否输出清屏序列");
    {
        std::ostringstream clearOut;
        ViewOptions clearOptions;
        clearOptions.clearScreen = true;
        ConsoleView clearView(colors, clearOptions, clearOut);
        clearView.BeginFrame();
        CHECK_EQ(clearOut.str(), std::string(kClearSequence));

        std::ostringstream appendOut;
        ViewOptions appendOptions;
        appendOptions.clearScreen = false;
        ConsoleView appendView(colors, appendOptions, appendOut);
        appendView.BeginFrame();
        CHECK(appendOut.str().empty());  // 追加模式下 BeginFrame 是空操作
    }

    CASE("View: 每帧以清屏开头，帧内只包含当前状态");
    {
        Game game(MakeLevel(), colors);
        game.Fire(3);  // RRRB -> 消除 3 个 R -> B -> 入球 G -> BG

        std::ostringstream out;
        ViewOptions options;
        options.clearScreen = true;
        ConsoleView view(colors, options, out);

        view.BeginFrame();
        view.Render(game);
        view.BeginFrame();
        view.Render(game);

        const std::string text = out.str();
        CHECK_EQ(text.find(kClearSequence), std::size_t(0));                     // 第 1 帧以清屏开头
        CHECK_EQ(CountOccurrences(text, kClearSequence), std::size_t(2));        // 两帧 = 两次清屏
        CHECK_EQ(CountOccurrences(text, std::string("得分：")), std::size_t(2));  // 每帧一份状态
        CHECK(text.find("轨道") != std::string::npos);
        CHECK(text.find("[B]") != std::string::npos);  // 当前球链 BG
        CHECK(text.find("[G]") != std::string::npos);
        CHECK(text.find("得分：30") != std::string::npos);
        CHECK(text.find("回合：1") != std::string::npos);
    }

    CASE("View: useColor 关闭时不输出颜色转义");
    {
        Game game(MakeLevel(), colors);
        std::ostringstream out;
        ViewOptions options;
        options.clearScreen = true;
        options.useColor = false;
        ConsoleView view(colors, options, out);

        view.BeginFrame();
        view.Render(game);
        view.RenderTurnResult(game.Drop(), game);

        const std::string text = out.str();
        CHECK(text.find("\x1b[", clearLength) == std::string::npos);  // 清屏序列之后无任何转义
    }

    CASE("View: useColor 开启时给已注册颜色加颜色转义");
    {
        Game game(MakeLevel(), colors);
        std::ostringstream out;
        ViewOptions options;
        options.useColor = true;
        ConsoleView view(colors, options, out);
        view.Render(game);
        CHECK(out.str().find("\x1b[31m") != std::string::npos);  // 链上的 R -> 红色
    }

    CASE("View: 未注册颜色不崩溃且不加颜色转义");
    {
        Game game(MakeLevel("ZR", "", "RG", 5), colors);

        std::ostringstream out;
        ViewOptions options;
        options.useColor = true;
        ConsoleView view(colors, options, out);
        view.Render(game);

        const std::string text = out.str();
        CHECK(text.find("[Z]") != std::string::npos);
        CHECK(text.find("\x1b[31m") != std::string::npos);  // 同一帧里的 R 仍然着色
    }

    CASE("View: 回合日志包含普通消除与连锁消除轮次");
    {
        Game game(MakeLevel("RRGGRR", "", "GGG", 8), colors);
        const zuma::core::TurnResult result = game.Fire(3);

        std::ostringstream out;
        ViewOptions options;
        ConsoleView view(colors, options, out);
        view.RenderTurnResult(result, game);

        const std::string text = out.str();
        CHECK(text.find("发射 [G]") != std::string::npos);
        CHECK(text.find("普通消除：3 个") != std::string::npos);
        CHECK(text.find("连锁消除 #2：4 个") != std::string::npos);
        CHECK(text.find("等待区已空") != std::string::npos);
        CHECK(text.find("手持更新") != std::string::npos);
    }

    CASE("View: 等待区数量与超长截断");
    {
        Game game(MakeLevel("R", "GYB", "RG", 8), colors);

        std::ostringstream full;
        ViewOptions fullOptions;
        ConsoleView fullView(colors, fullOptions, full);
        fullView.Render(game);
        CHECK(full.str().find("等待区（3）：G Y B") != std::string::npos);

        std::ostringstream clipped;
        ViewOptions clippedOptions;
        clippedOptions.feedDisplayLimit = 2;
        ConsoleView clippedView(colors, clippedOptions, clipped);
        clippedView.Render(game);
        CHECK(clipped.str().find("等待区（3）：G Y ...(还有 1 个)") != std::string::npos);
    }

    CASE("View: 状态与结束画面");
    {
        Game game(MakeLevel("BGY", "", "RG", 3), colors);
        game.Fire(1);  // RBGY -> 4 > 3，立即判负

        std::ostringstream out;
        ViewOptions options;
        ConsoleView view(colors, options, out);
        view.Render(game);
        view.RenderOutcome(game);

        const std::string text = out.str();
        CHECK(text.find("状态：失败") != std::string::npos);
        CHECK(text.find("游戏失败：轨道长度 4 超过上限 3") != std::string::npos);
    }

    CASE("View: 关卡菜单与警告");
    {
        std::ostringstream out;
        ViewOptions options;
        ConsoleView view(colors, options, out);

        std::vector<zuma::core::Level> levels;
        levels.push_back(MakeLevel());
        levels.push_back(MakeLevel("RRR", "", "RG", 6));
        view.RenderLevelMenu(levels);

        std::vector<std::string> warnings;
        warnings.push_back("颜色 'Z' 未在颜色表中注册");
        view.RenderWarnings(warnings);

        const std::string text = out.str();
        CHECK(text.find("1) 视图测试关") != std::string::npos);
        CHECK(text.find("2) 视图测试关") != std::string::npos);
        CHECK(text.find("上限 8") != std::string::npos);
        CHECK(text.find("颜色 'Z' 未在颜色表中注册") != std::string::npos);
    }
}
