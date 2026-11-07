#include "core/LevelLoader.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "util/StringUtil.h"

namespace zuma::core {
namespace {

std::string ErrorAt(std::size_t lineNumber, const std::string& reason) {
    return "第 " + std::to_string(lineNumber) + " 行：" + reason;
}

// 文件名（去目录与扩展名）作为缺省关卡名。
std::string FileStem(const std::string& path) {
    const std::filesystem::path p(path);
    return p.stem().string();
}

bool HasRunOfThreeOrMore(const std::string& colors) {
    std::size_t runLength = 0;
    char previous = '\0';
    for (char color : colors) {
        if (runLength != 0 && color == previous) {
            ++runLength;
        } else {
            runLength = 1;
            previous = color;
        }
        if (runLength >= 3) {
            return true;
        }
    }
    return false;
}

bool IsSupportedKey(const std::string& key) {
    return key == "NAME" || key == "ACTIVE" || key == "FEED" || key == "AMMO" ||
           key == "MAX_LENGTH";
}

}  // namespace

std::optional<LevelLoadReport> LevelLoader::Parse(std::istream& in, const std::string& displayName,
                                                  std::string& error) {
    Level level;
    level.sourcePath = displayName;
    level.name = FileStem(displayName);

    LevelLoadReport report;
    bool seenName = false;
    bool seenActive = false;
    bool seenFeed = false;
    bool seenAmmo = false;
    bool seenMaxLength = false;
    bool hasContent = false;

    std::string rawLine;
    std::size_t lineNumber = 0;
    while (std::getline(in, rawLine)) {
        ++lineNumber;

        // 首行可能带 UTF-8 BOM
        if (lineNumber == 1 && rawLine.size() >= 3 &&
            static_cast<unsigned char>(rawLine[0]) == 0xEF &&
            static_cast<unsigned char>(rawLine[1]) == 0xBB &&
            static_cast<unsigned char>(rawLine[2]) == 0xBF) {
            rawLine.erase(0, 3);
        }

        const std::string line = util::Trim(rawLine);
        if (line.empty() || line[0] == '#') {
            continue;
        }
        hasContent = true;

        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) {
            error = ErrorAt(lineNumber, "缺少 '='，应为 KEY=VALUE 形式");
            return std::nullopt;
        }

        const std::string key = util::ToUpperAscii(util::Trim(line.substr(0, equals)));
        std::string value = util::Trim(line.substr(equals + 1));
        const std::size_t comment = value.find('#');
        if (comment != std::string::npos) {
            value = util::Trim(value.substr(0, comment));
        }

        if (!IsSupportedKey(key)) {
            error = ErrorAt(lineNumber,
                            "未知字段 \"" + key +
                                "\"（支持 NAME / ACTIVE / FEED / AMMO / MAX_LENGTH）");
            return std::nullopt;
        }

        bool* seenFlag = nullptr;
        if (key == "NAME") {
            seenFlag = &seenName;
        } else if (key == "ACTIVE") {
            seenFlag = &seenActive;
        } else if (key == "FEED") {
            seenFlag = &seenFeed;
        } else if (key == "AMMO") {
            seenFlag = &seenAmmo;
        } else {
            seenFlag = &seenMaxLength;
        }
        if (*seenFlag) {
            report.warnings.push_back(ErrorAt(lineNumber, "字段 " + key + " 重复出现，以最后一次为准"));
        }
        *seenFlag = true;

        if (key == "NAME") {
            if (!value.empty()) {
                level.name = value;
            }
            continue;
        }

        if (key == "MAX_LENGTH") {
            std::size_t parsed = 0;
            if (!util::ParseUnsigned(value, parsed) || parsed < kMinMaxLength ||
                parsed > kMaxMaxLength) {
                error = ErrorAt(lineNumber,
                                "MAX_LENGTH 必须是 " + std::to_string(kMinMaxLength) + ".." +
                                    std::to_string(kMaxMaxLength) + " 之间的整数，实际为 \"" +
                                    value + "\"");
                return std::nullopt;
            }
            level.maxLength = parsed;
            continue;
        }

        // ACTIVE / FEED / AMMO：只允许颜色字母
        std::string colors;
        colors.reserve(value.size());
        for (char ch : value) {
            if (!util::IsAsciiAlpha(ch)) {
                std::string shown(1, ch);
                error = ErrorAt(lineNumber, key + " 中出现非法颜色字符 '" + shown + "'（只允许 A-Za-z）");
                return std::nullopt;
            }
            colors.push_back(util::ToUpperAscii(std::string(1, ch))[0]);
        }
        if (key == "ACTIVE") {
            level.active = colors;
        } else if (key == "FEED") {
            level.feed = colors;
        } else {
            level.ammo = colors;
        }
    }

    if (!hasContent) {
        error = "关卡文件为空（或只有注释）";
        return std::nullopt;
    }

    std::vector<std::string> missing;
    if (!seenActive) {
        missing.push_back("ACTIVE");
    }
    if (!seenFeed) {
        missing.push_back("FEED");
    }
    if (!seenAmmo) {
        missing.push_back("AMMO");
    }
    if (!seenMaxLength) {
        missing.push_back("MAX_LENGTH");
    }
    if (!missing.empty()) {
        error = "缺少必需字段：" + util::Join(missing, "、");
        return std::nullopt;
    }

    if (level.ammo.empty()) {
        error = "AMMO 不能为空（弹药循环规律至少需要 1 个字符）";
        return std::nullopt;
    }

    if (level.active.size() > level.maxLength) {
        error = "ACTIVE 长度 " + std::to_string(level.active.size()) + " 超过 MAX_LENGTH " +
                std::to_string(level.maxLength) + "（开局即为必败态，拒绝加载）";
        return std::nullopt;
    }

    if (HasRunOfThreeOrMore(level.active)) {
        report.warnings.push_back(
            "ACTIVE 中已存在长度 >= 3 的同色连续段；开局不会自动消除，首次发射后可能立即触发消除");
    }

    report.level = level;
    return report;
}

std::optional<LevelLoadReport> LevelLoader::LoadFile(const std::string& path, std::string& error) {
    std::ifstream input(path);
    if (!input) {
        error = "无法打开关卡文件：" + path;
        return std::nullopt;
    }
    return Parse(input, path, error);
}

std::vector<Level> LevelLoader::LoadDirectory(const std::string& directory,
                                              std::vector<std::string>& errors) {
    std::vector<Level> levels;

    std::error_code code;
    if (!std::filesystem::is_directory(directory, code)) {
        return levels;
    }

    std::vector<std::filesystem::path> files;
    for (const std::filesystem::directory_entry& entry :
         std::filesystem::directory_iterator(directory, code)) {
        if (!entry.is_regular_file(code)) {
            continue;
        }
        std::string extension = util::ToUpperAscii(entry.path().extension().string());
        if (extension == ".TXT") {
            files.push_back(entry.path());
        }
    }
    std::sort(files.begin(), files.end());

    for (const std::filesystem::path& file : files) {
        std::string error;
        std::optional<LevelLoadReport> report = LoadFile(file.string(), error);
        if (!report) {
            errors.push_back(error);
            continue;
        }
        levels.push_back(report->level);
    }
    return levels;
}

}  // namespace zuma::core
