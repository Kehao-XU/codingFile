#pragma once

#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cli/ConsoleView.h"
#include "core/ColorRegistry.h"
#include "core/Level.h"

namespace zuma::cli {

// 应用外壳：关卡选择 + 游戏主循环 + 一局结束后的重玩/换关卡/退出。
class ConsoleApp {
public:
    ConsoleApp(std::istream& in, std::ostream& out, bool useColor, bool clearScreen,
               const std::vector<std::string>& levelPaths);

    int Run();

private:
    // 一局结束后玩家的选择。
    enum class PostGameAction { Replay, ChangeLevel, Quit, Exit };

    bool DiscoverLevels();
    std::optional<core::Level> ChooseLevel();
    PostGameAction PlayOneGame(const core::Level& level);
    PostGameAction AskPostGameAction(const core::Game& game);
    std::optional<std::string> ReadLine();
    // 在下一帧（无论菜单帧还是开局帧）顶部显示一次关卡加载阶段的提示
    void RenderStartupMessages();

    std::istream& in_;
    std::ostream& out_;
    core::ColorRegistry colors_;
    std::unique_ptr<ConsoleView> view_;
    std::vector<std::string> levelPaths_;
    std::vector<core::Level> levels_;
    // 关卡加载阶段的提示，留到第一帧（清屏之后）显示，避免被清屏吞掉。
    std::vector<std::string> startupWarnings_;
    std::vector<std::string> startupErrors_;
};

}  // namespace zuma::cli
