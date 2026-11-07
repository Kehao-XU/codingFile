#include <string>

#include "cli/CommandParser.h"
#include "test_harness.h"

void RunCommandParserTests() {
    using zuma::cli::CommandParser;
    using zuma::cli::CommandType;

    CASE("Parser: FIRE 及其缩写");
    {
        CHECK(CommandParser::Parse("FIRE 3").type == CommandType::Fire);
        CHECK_EQ(CommandParser::Parse("FIRE 3").arg, std::size_t(3));
        CHECK(CommandParser::Parse("fire 3").type == CommandType::Fire);
        CHECK(CommandParser::Parse("F 3").type == CommandType::Fire);
        CHECK(CommandParser::Parse("  F\xE3\x80\x80 4  ").type == CommandType::Fire);  // 全角空格
        CHECK_EQ(CommandParser::Parse("  F\xE3\x80\x80 4  ").arg, std::size_t(4));
    }

    CASE("Parser: FIRE 非法语法（不得消耗彩球）");
    {
        for (const char* line : {"FIRE", "F", "FIRE abc", "FIRE -1", "FIRE 0", "FIRE 1 2",
                                 "FIRE 1.5", "FI 3", "FIRE 999999999999999999999999"}) {
            const zuma::cli::Command command = CommandParser::Parse(line);
            CHECK(command.type == CommandType::Invalid);
            CHECK_FALSE(command.error.empty());
        }
    }

    CASE("Parser: DROP / SHOW / HELP / UNDO / HINT / QUIT");
    {
        CHECK(CommandParser::Parse("DROP").type == CommandType::Drop);
        CHECK(CommandParser::Parse("d").type == CommandType::Drop);
        CHECK(CommandParser::Parse("SHOW").type == CommandType::Show);
        CHECK(CommandParser::Parse("s").type == CommandType::Show);
        CHECK(CommandParser::Parse("HELP").type == CommandType::Help);
        CHECK(CommandParser::Parse("h").type == CommandType::Help);
        CHECK(CommandParser::Parse("?").type == CommandType::Help);
        CHECK(CommandParser::Parse("UNDO").type == CommandType::Undo);
        CHECK(CommandParser::Parse("u").type == CommandType::Undo);
        CHECK(CommandParser::Parse("HINT").type == CommandType::Hint);
        CHECK(CommandParser::Parse("QUIT").type == CommandType::Quit);
        CHECK(CommandParser::Parse("q").type == CommandType::Quit);
        CHECK(CommandParser::Parse("EXIT").type == CommandType::Quit);
    }

    CASE("Parser: 无参命令带参数视为非法");
    {
        CHECK(CommandParser::Parse("DROP 1").type == CommandType::Invalid);
        CHECK(CommandParser::Parse("HELP ME").type == CommandType::Invalid);
        CHECK(CommandParser::Parse("QUIT NOW").type == CommandType::Invalid);
    }

    CASE("Parser: 空行与未知命令");
    {
        CHECK(CommandParser::Parse("").type == CommandType::Invalid);
        CHECK(CommandParser::Parse("     ").type == CommandType::Invalid);
        CHECK(CommandParser::Parse("FLY 3").type == CommandType::Invalid);
        CHECK(CommandParser::Parse("ZZZ").type == CommandType::Invalid);
        CHECK(CommandParser::Parse("FLY 3").error.find("未知命令") != std::string::npos);
    }
}
