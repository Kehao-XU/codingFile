#pragma once

#include <cstddef>
#include <deque>
#include <iosfwd>
#include <string>
#include <vector>

#include "core/Ball.h"
#include "core/ColorRegistry.h"
#include "core/Game.h"
#include "core/Level.h"

namespace zuma::cli {

struct ViewOptions {
    bool useColor = false;              // 终端不支持 ANSI 或用户 --no-color 时为 false
    bool clearScreen = false;           // 每帧清屏重绘（终端支持 ANSI 时默认开启）
    std::size_t feedDisplayLimit = 32;  // 等待区最多显示多少个球
};

// 表现层：唯一允许写标准输出的地方。
class ConsoleView {
public:
    ConsoleView(const core::ColorRegistry& colors, ViewOptions options, std::ostream& out);

    // 每帧开始：清屏并把光标移回左上角，使屏幕上只保留"当前状态"
    // （clearScreen 为 false 时为空操作，退化为逐帧追加的历史记录模式）。
    void BeginFrame() const;

    void RenderLevelMenu(const std::vector<core::Level>& levels) const;
    void RenderWarnings(const std::vector<std::string>& warnings) const;
    void Render(const core::Game& game) const;
    void RenderTurnResult(const core::TurnResult& result, const core::Game& game) const;
    void RenderOutcome(const core::Game& game) const;
    void RenderHelp() const;
    void RenderError(const std::string& message) const;
    void RenderInfo(const std::string& message) const;
    void Prompt(const std::string& text = "  命令 > ") const;

private:
    std::string BallText(char color) const;
    std::string FeedText(const std::deque<core::Ball>& feed) const;
    static const char* StatusName(core::GameStatus status);

    const core::ColorRegistry& colors_;
    ViewOptions options_;
    std::ostream& out_;
};

}  // namespace zuma::cli
