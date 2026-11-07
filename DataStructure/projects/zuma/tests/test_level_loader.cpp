#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

#include "core/LevelLoader.h"
#include "test_harness.h"

namespace {

std::optional<zuma::core::LevelLoadReport> ParseText(const std::string& text, std::string& error) {
    std::istringstream stream(text);
    return zuma::core::LevelLoader::Parse(stream, "unit-test.txt", error);
}

std::string JoinPath(const std::string& directory, const std::string& name) {
    return (std::filesystem::path(directory) / name).string();
}

}  // namespace

void RunLevelLoaderTests(const std::string& projectDirectory) {
    using zuma::core::Level;
    using zuma::core::LevelLoadReport;
    using zuma::core::LevelLoader;

    CASE("LevelLoader: 正常关卡");
    {
        std::string error;
        const std::string text =
            "ACTIVE=RRBBRR\nFEED=GBYPRGBGYRBPYG\nAMMO=BBGYRRGBYPRG\nMAX_LENGTH=12\n";
        const std::optional<LevelLoadReport> report = ParseText(text, error);
        CHECK(report.has_value());
        if (report) {
            CHECK_EQ(report->level.active, std::string("RRBBRR"));
            CHECK_EQ(report->level.feed, std::string("GBYPRGBGYRBPYG"));
            CHECK_EQ(report->level.ammo, std::string("BBGYRRGBYPRG"));
            CHECK_EQ(report->level.maxLength, std::size_t(12));
            CHECK_EQ(report->level.name, std::string("unit-test"));  // 缺省用文件名
            CHECK(report->warnings.empty());
        }
    }

    CASE("LevelLoader: 空行 / 注释 / 行尾注释 / 小写键名 / 空 FEED");
    {
        std::string error;
        const std::string text =
            "\n"
            "# 这是注释\n"
            "name=教学关   # 行尾注释\n"
            "active=rrgg\n"
            "feed=\n"
            "ammo=GGG\n"
            "max_length=8\n";
        const std::optional<LevelLoadReport> report = ParseText(text, error);
        CHECK(report.has_value());
        if (report) {
            CHECK_EQ(report->level.name, std::string("教学关"));
            CHECK_EQ(report->level.active, std::string("RRGG"));
            CHECK_EQ(report->level.feed, std::string(""));
            CHECK_EQ(report->level.ammo, std::string("GGG"));
            CHECK_EQ(report->level.maxLength, std::size_t(8));
        }
    }

    CASE("LevelLoader: BOM + CRLF（Windows 记事本保存格式）");
    {
        std::string error;
        const std::string text =
            "\xEF\xBB\xBF" "ACTIVE=RRB\r\nFEED=G\r\nAMMO=RG\r\nMAX_LENGTH=5\r\n";
        const std::optional<LevelLoadReport> report = ParseText(text, error);
        CHECK(report.has_value());
        if (report) {
            CHECK_EQ(report->level.active, std::string("RRB"));
            CHECK_EQ(report->level.feed, std::string("G"));
            CHECK_EQ(report->level.maxLength, std::size_t(5));
        }
    }

    CASE("LevelLoader: 缺少必需字段");
    {
        std::string error;
        const std::string text = "ACTIVE=RRB\nFEED=G\nMAX_LENGTH=5\n";
        const std::optional<LevelLoadReport> report = ParseText(text, error);
        CHECK_FALSE(report.has_value());
        CHECK(error.find("AMMO") != std::string::npos);
    }

    CASE("LevelLoader: MAX_LENGTH 非法");
    {
        for (const char* bad : {"abc", "0", "1000", "-3", "12abc", ""}) {
            std::string error;
            const std::string text =
                std::string("ACTIVE=RRB\nFEED=\nAMMO=RG\nMAX_LENGTH=") + bad + "\n";
            const std::optional<LevelLoadReport> report = ParseText(text, error);
            CHECK_FALSE(report.has_value());
            CHECK(error.find("MAX_LENGTH") != std::string::npos);
        }
    }

    CASE("LevelLoader: ACTIVE 出现非法颜色字符");
    {
        std::string error;
        const std::string text = "ACTIVE=R1B\nFEED=\nAMMO=RG\nMAX_LENGTH=5\n";
        const std::optional<LevelLoadReport> report = ParseText(text, error);
        CHECK_FALSE(report.has_value());
        CHECK(error.find("非法颜色字符") != std::string::npos);
        CHECK(error.find("第 1 行") != std::string::npos);
    }

    CASE("LevelLoader: ACTIVE 长度超过 MAX_LENGTH 时拒绝加载");
    {
        std::string error;
        const std::string text = "ACTIVE=RRBBRR\nFEED=\nAMMO=RG\nMAX_LENGTH=3\n";
        const std::optional<LevelLoadReport> report = ParseText(text, error);
        CHECK_FALSE(report.has_value());
        CHECK(error.find("超过 MAX_LENGTH") != std::string::npos);
    }

    CASE("LevelLoader: 初始三连只给警告，不阻断");
    {
        std::string error;
        const std::string text = "ACTIVE=RRRB\nFEED=\nAMMO=RG\nMAX_LENGTH=6\n";
        const std::optional<LevelLoadReport> report = ParseText(text, error);
        CHECK(report.has_value());
        if (report) {
            CHECK_EQ(report->warnings.size(), std::size_t(1));
        }
    }

    CASE("LevelLoader: 未知字段 / 缺少等号 / 空文件 / AMMO 为空");
    {
        std::string error;
        CHECK_FALSE(ParseText("ACTIVE=RRB\nCOLOR=1\nFEED=\nAMMO=RG\nMAX_LENGTH=5\n", error)
                        .has_value());
        CHECK(error.find("未知字段") != std::string::npos);

        CHECK_FALSE(ParseText("ACTIVE=RRB\nFEED=\nAMMO RG\nMAX_LENGTH=5\n", error).has_value());
        CHECK(error.find("'='") != std::string::npos);

        CHECK_FALSE(ParseText("\n# 只有注释\n", error).has_value());

        CHECK_FALSE(ParseText("ACTIVE=RRB\nFEED=\nAMMO=\nMAX_LENGTH=5\n", error).has_value());
        CHECK(error.find("AMMO") != std::string::npos);
    }

    CASE("LevelLoader: 重复字段以最后一次为准并给出警告");
    {
        std::string error;
        const std::string text =
            "ACTIVE=RRB\nACTIVE=RGB\nFEED=\nAMMO=RG\nMAX_LENGTH=5\n";
        const std::optional<LevelLoadReport> report = ParseText(text, error);
        CHECK(report.has_value());
        if (report) {
            CHECK_EQ(report->level.active, std::string("RGB"));
            CHECK_EQ(report->warnings.size(), std::size_t(1));
        }
    }

    CASE("LevelLoader: 读取真实关卡文件");
    {
        if (projectDirectory.empty()) {
            zuma::test::Fail("未提供项目目录，跳过", __FILE__, __LINE__);
        } else {
            std::string error;
            const std::optional<LevelLoadReport> level1 =
                LevelLoader::LoadFile(JoinPath(JoinPath(projectDirectory, "levels"), "level1.txt"),
                                      error);
            CHECK(level1.has_value());
            if (level1) {
                CHECK_EQ(level1->level.active, std::string("RRBBRR"));
                CHECK_EQ(level1->level.maxLength, std::size_t(12));
            }

            const std::optional<LevelLoadReport> level2 =
                LevelLoader::LoadFile(JoinPath(JoinPath(projectDirectory, "levels"), "level2.txt"),
                                      error);
            CHECK(level2.has_value());
            if (level2) {
                CHECK_EQ(level2->level.maxLength, std::size_t(15));
            }

            std::vector<std::string> errors;
            const std::vector<Level> levels =
                LevelLoader::LoadDirectory(JoinPath(projectDirectory, "levels"), errors);
            CHECK(errors.empty());
            CHECK_EQ(levels.size(), std::size_t(3));  // level1 / level2 / demo
        }
    }
}
