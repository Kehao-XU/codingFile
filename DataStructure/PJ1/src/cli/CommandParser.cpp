#include "cli/CommandParser.h"

#include <vector>

#include "util/StringUtil.h"

namespace zuma::cli {
namespace {

Command Invalid(const std::string& reason) {
    Command command;
    command.type = CommandType::Invalid;
    command.error = reason;
    return command;
}

// 不接受参数的命令（DROP/SHOW/HELP/UNDO/HINT/QUIT）。
Command NoArgument(CommandType type, std::size_t extraTokens, const char* name) {
    if (extraTokens != 0) {
        return Invalid(std::string(name) + " 不接受参数（输入 HELP 查看用法）");
    }
    Command command;
    command.type = type;
    return command;
}

}  // namespace

Command CommandParser::Parse(const std::string& line) {
    const std::vector<std::string> tokens = util::SplitWhitespace(line);
    if (tokens.empty()) {
        return Invalid("请输入命令（输入 HELP 查看帮助）");
    }

    const std::string head = util::ToUpperAscii(tokens[0]);
    const std::size_t extra = tokens.size() - 1;

    if (head == "F" || head == "FIRE") {
        if (extra == 0) {
            return Invalid("FIRE 需要一个位置参数，例如：FIRE 3");
        }
        if (extra > 1) {
            return Invalid("FIRE 只接受一个位置参数，例如：FIRE 3");
        }
        std::size_t position = 0;
        if (!util::ParseUnsigned(tokens[1], position) || position == 0) {
            return Invalid("位置必须是正整数，实际输入为 \"" + tokens[1] + "\"");
        }
        Command command;
        command.type = CommandType::Fire;
        command.arg = position;
        return command;
    }

    if (head == "D" || head == "DROP") {
        return NoArgument(CommandType::Drop, extra, "DROP");
    }
    if (head == "S" || head == "SHOW") {
        return NoArgument(CommandType::Show, extra, "SHOW");
    }
    if (head == "H" || head == "HELP" || head == "?") {
        return NoArgument(CommandType::Help, extra, "HELP");
    }
    if (head == "U" || head == "UNDO") {
        return NoArgument(CommandType::Undo, extra, "UNDO");
    }
    if (head == "HINT") {
        return NoArgument(CommandType::Hint, extra, "HINT");
    }
    if (head == "Q" || head == "QUIT" || head == "EXIT") {
        return NoArgument(CommandType::Quit, extra, "QUIT");
    }

    return Invalid("未知命令 \"" + tokens[0] + "\"（输入 HELP 查看帮助）");
}

}  // namespace zuma::cli
