cmake_minimum_required(VERSION 3.16)
project(zuma LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# 规则引擎 + 表现层（不含 main）
add_library(zuma_core STATIC
    src/util/StringUtil.cpp
    src/util/ConsoleColor.cpp
    src/core/ColorRegistry.cpp
    src/core/Track.cpp
    src/core/EliminationEngine.cpp
    src/core/FeedQueue.cpp
    src/core/AmmoGenerator.cpp
    src/core/ScoreBoard.cpp
    src/core/LevelLoader.cpp
    src/core/Game.cpp
    src/cli/CommandParser.cpp
    src/cli/ConsoleView.cpp
    src/cli/ConsoleApp.cpp
)
target_include_directories(zuma_core PUBLIC src)

# 游戏可执行文件
add_executable(zuma src/main.cpp)
target_link_libraries(zuma PRIVATE zuma_core)

# 把关卡文件复制到可执行文件旁，保证解压后直接可运行
add_custom_command(TARGET zuma POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/levels"
            "$<TARGET_FILE_DIR:zuma>/levels")

# 单元测试
enable_testing()
add_executable(zuma_tests
    tests/test_main.cpp
    tests/test_track.cpp
    tests/test_elimination.cpp
    tests/test_ammo_feed.cpp
    tests/test_level_loader.cpp
    tests/test_command_parser.cpp
    tests/test_game_turns.cpp
    tests/test_console_view.cpp
)
target_link_libraries(zuma_tests PRIVATE zuma_core)
add_test(NAME zuma_unit_tests COMMAND zuma_tests "${CMAKE_CURRENT_SOURCE_DIR}")
