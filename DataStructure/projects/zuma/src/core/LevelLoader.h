#pragma once

#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string>
#include <vector>

#include "core/Level.h"

namespace zuma::core {

// 关卡文件读取与校验。字段格式为 KEY=VALUE，支持空行、'#' 注释、行尾注释、UTF-8 BOM 与 CRLF。
class LevelLoader {
public:
    static constexpr std::size_t kMinMaxLength = 1;
    static constexpr std::size_t kMaxMaxLength = 999;

    // 从字符流解析（纯函数，便于单元测试）。
    static std::optional<LevelLoadReport> Parse(std::istream& in, const std::string& displayName,
                                                std::string& error);
    static std::optional<LevelLoadReport> LoadFile(const std::string& path, std::string& error);

    // 扫描目录下所有 *.txt（按文件名排序）；无法读取的文件记入 errors 并跳过。
    static std::vector<Level> LoadDirectory(const std::string& directory,
                                            std::vector<std::string>& errors);
};

}  // namespace zuma::core
