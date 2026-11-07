#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace zuma::core {

// 关卡数据（保留原始字段，便于回显与调试）。
struct Level {
    std::string name;        // 显示名（NAME 字段，缺省用文件名）
    std::string sourcePath;  // 来源文件路径
    std::string active;      // ACTIVE：开局已在轨道的彩球
    std::string feed;        // FEED：之后依次进入轨道的彩球
    std::string ammo;        // AMMO：弹药循环规律
    std::size_t maxLength = 0;  // MAX_LENGTH：轨道允许的最大彩球数
};

// 校验结果：合法关卡 + 若干警告（警告不阻断加载）。
struct LevelLoadReport {
    Level level;
    std::vector<std::string> warnings;
};

}  // namespace zuma::core
