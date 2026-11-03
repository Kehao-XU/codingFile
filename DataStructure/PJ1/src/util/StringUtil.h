#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace zuma::util {

// 去除首尾空白（ASCII 空白 + UTF-8 全角空格 U+3000）。
std::string Trim(const std::string& text);

// 仅转换 ASCII 小写字母为大写，其余字节原样保留（不破坏 UTF-8）。
std::string ToUpperAscii(const std::string& text);

// 严格解析非负整数：允许首尾空白，禁止符号、空串与溢出。
bool ParseUnsigned(const std::string& text, std::size_t& out);

// 按空白切分（连续空白视为一个分隔符，忽略首尾空白；全角空格同样作为分隔符）。
std::vector<std::string> SplitWhitespace(const std::string& text);

std::string Join(const std::vector<std::string>& parts, const std::string& separator);

// 判断字符是否为关卡文件中允许的颜色字母（A-Za-z）。
bool IsAsciiAlpha(char ch);

}  // namespace zuma::util
