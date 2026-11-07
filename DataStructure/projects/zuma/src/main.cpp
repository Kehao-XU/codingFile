#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "cli/ConsoleApp.h"
#include "util/ConsoleColor.h"

namespace {

void PrintUsage() {
    std::cout << "祖玛彩球连锁消除（命令行版）\n"
              << "用法：zuma [选项] [关卡文件...]\n"
              << "  关卡文件   直接进入指定关卡（可给出多个，形成一个关卡菜单）\n"
              << "  --clear    强制每帧清屏重绘\n"
              << "  --no-clear 关闭清屏，改为逐帧追加（保留完整历史记录，便于录制/粘贴）\n"
              << "  --no-color 关闭 ANSI 彩色输出\n"
              << "  -h, --help 显示本帮助\n"
              << "不带参数运行时，会自动在当前目录（及其上级目录）的 levels/ 中查找关卡。\n"
              << "默认行为：在支持 ANSI 的交互终端上清屏重绘，屏幕只显示当前状态与本回合日志；\n"
              << "输出被重定向（管道/文件）时自动改为追加模式。\n";
}

}  // namespace

int main(int argc, char** argv) {
    zuma::util::EnableUtf8Console();

    std::vector<std::string> levelPaths;
    bool colorRequested = true;
    int clearMode = 0;  // -1 强制关闭，0 自动，1 强制开启

    for (int index = 1; index < argc; ++index) {
        const std::string argument = argv[index];
        if (argument == "--no-color") {
            colorRequested = false;
        } else if (argument == "--no-clear") {
            clearMode = -1;
        } else if (argument == "--clear") {
            clearMode = 1;
        } else if (argument == "-h" || argument == "--help") {
            PrintUsage();
            return 0;
        } else if (!argument.empty() && argument[0] == '-') {
            std::cerr << "未知选项：" << argument << "\n";
            PrintUsage();
            return 1;
        } else {
            levelPaths.push_back(argument);
        }
    }

    if (colorRequested && std::getenv("NO_COLOR") != nullptr) {
        colorRequested = false;
    }

    // ANSI 能力检测：输出被重定向或终端不支持 VT 时，颜色与清屏都会自动退化。
    const bool ansiAvailable =
        zuma::util::StdoutIsTerminal() && zuma::util::EnableAnsiOnStdout();
    const bool useColor = colorRequested && ansiAvailable;
    const bool useClear = clearMode == 1 || (clearMode == 0 && ansiAvailable);

    try {
        zuma::cli::ConsoleApp app(std::cin, std::cout, useColor, useClear, levelPaths);
        return app.Run();
    } catch (const std::exception& error) {
        std::cerr << "程序异常终止：" << error.what() << "\n";
        return 2;
    }
}
