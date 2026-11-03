#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace zuma::core {

// 一种颜色的显示信息。ansiFg 为 ANSI 转义前缀（空串表示不染色）。
struct ColorInfo {
    char key = '?';
    const char* displayName = "未注册";
    const char* ansiFg = "";
};

// 颜色字符 -> 显示样式的查表（哈希表）。未注册颜色走兜底样式，不抛异常。
class ColorRegistry {
public:
    ColorRegistry();

    bool Contains(char key) const;
    const ColorInfo& Get(char key) const;
    static const ColorInfo& Fallback();

    std::vector<char> Keys() const;

private:
    std::unordered_map<char, ColorInfo> table_;
};

}  // namespace zuma::core
