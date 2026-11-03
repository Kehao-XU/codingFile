#pragma once

#include <cstddef>
#include <string>

namespace zuma::cli {

enum class CommandType { Fire, Drop, Show, Help, Undo, Hint, Quit, Invalid };

struct Command {
    CommandType type = CommandType::Invalid;
    std::size_t arg = 0;   // Fire 的位置参数
    std::string error;
};

// 命令解析器：纯函数、无副作用。只负责“语法”，位置合法性由 core::Game 判定。
class CommandParser {
public:
    static Command Parse(const std::string& line);
};

}  // namespace zuma::cli
