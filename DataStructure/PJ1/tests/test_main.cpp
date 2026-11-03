#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>

#include "test_harness.h"

void RunTrackTests();
void RunEliminationTests();
void RunAmmoFeedTests();
void RunLevelLoaderTests(const std::string& projectDirectory);
void RunCommandParserTests();
void RunGameTurnTests();
void RunConsoleViewTests();

namespace {

// 从当前目录与可执行文件所在目录出发向上查找含 levels/level1.txt 的项目根目录，
// 这样无论从哪里运行测试都能找到真实关卡文件。
std::string LocateProjectDirectory(const std::string& executablePath) {
    namespace fs = std::filesystem;

    std::vector<fs::path> starts;
    std::error_code code;
    starts.push_back(fs::current_path(code));
    if (!executablePath.empty()) {
        starts.push_back(fs::absolute(fs::path(executablePath), code).parent_path());
    }

    for (fs::path start : starts) {
        for (int depth = 0; depth < 6; ++depth) {
            if (fs::exists(start / "levels" / "level1.txt", code)) {
                return start.string();
            }
            if (!start.has_parent_path()) {
                break;
            }
            start = start.parent_path();
        }
    }
    return "";
}

}  // namespace

int main(int argc, char** argv) {
    std::string projectDirectory = argc > 1 ? argv[1] : LocateProjectDirectory(argv[0]);
    if (projectDirectory.empty()) {
        projectDirectory = LocateProjectDirectory("");
    }

    std::printf("==== 祖玛彩球 单元测试 ====\n");
    if (projectDirectory.empty()) {
        std::printf("（未找到项目目录，跳过依赖真实关卡文件的用例）\n");
    } else {
        std::printf("项目目录：%s\n", projectDirectory.c_str());
    }

    RunTrackTests();
    RunEliminationTests();
    RunAmmoFeedTests();
    RunLevelLoaderTests(projectDirectory);
    RunCommandParserTests();
    RunGameTurnTests();
    RunConsoleViewTests();

    std::printf("==== 检查 %d 项，失败 %d 项 ====\n", zuma::test::g_checks, zuma::test::g_failures);
    return zuma::test::g_failures == 0 ? 0 : 1;
}
