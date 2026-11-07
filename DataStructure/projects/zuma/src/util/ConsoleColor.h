#pragma once

namespace zuma::util {

// 标准输出是否连接到终端（被重定向到文件/管道时返回 false）。
bool StdoutIsTerminal();

// 标准输入是否连接到终端（管道/重定向时为 false）。
bool StdinIsTerminal();

// 尝试在 Windows 控制台开启 ANSI 转义序列（VT）处理；其它平台恒为 true。
bool EnableAnsiOnStdout();

// Windows 下把控制台代码页切到 UTF-8，保证中文与彩色输出正常；其它平台无操作。
void EnableUtf8Console();

}  // namespace zuma::util
