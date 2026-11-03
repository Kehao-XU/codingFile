#include "util/StringUtil.h"

#include <limits>

namespace zuma::util {
namespace {

bool IsAsciiSpace(unsigned char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\v' || ch == '\f';
}

// UTF-8 全角空格 U+3000 的字节序列。
bool IsFullWidthSpaceAt(const std::string& text, std::size_t index) {
    return index + 3 <= text.size() &&
           static_cast<unsigned char>(text[index]) == 0xE3 &&
           static_cast<unsigned char>(text[index + 1]) == 0x80 &&
           static_cast<unsigned char>(text[index + 2]) == 0x80;
}

bool IsWhitespaceAt(const std::string& text, std::size_t index, std::size_t& width) {
    if (index >= text.size()) {
        return false;
    }
    if (IsAsciiSpace(static_cast<unsigned char>(text[index]))) {
        width = 1;
        return true;
    }
    if (IsFullWidthSpaceAt(text, index)) {
        width = 3;
        return true;
    }
    return false;
}

}  // namespace

std::string Trim(const std::string& text) {
    std::size_t begin = 0;
    std::size_t end = text.size();
    std::size_t width = 0;

    while (begin < end && IsWhitespaceAt(text, begin, width)) {
        begin += width;
    }
    while (end > begin) {
        std::size_t index = end == 0 ? 0 : end - 1;
        if (end - begin >= 3 && IsFullWidthSpaceAt(text, end - 3)) {
            end -= 3;
            continue;
        }
        if (IsAsciiSpace(static_cast<unsigned char>(text[index]))) {
            --end;
            continue;
        }
        break;
    }
    return text.substr(begin, end - begin);
}

std::string ToUpperAscii(const std::string& text) {
    std::string result = text;
    for (char& ch : result) {
        if (ch >= 'a' && ch <= 'z') {
            ch = static_cast<char>(ch - 'a' + 'A');
        }
    }
    return result;
}

bool ParseUnsigned(const std::string& text, std::size_t& out) {
    const std::string trimmed = Trim(text);
    if (trimmed.empty()) {
        return false;
    }
    std::size_t value = 0;
    for (char ch : trimmed) {
        if (ch < '0' || ch > '9') {
            return false;
        }
        const std::size_t digit = static_cast<std::size_t>(ch - '0');
        if (value > (std::numeric_limits<std::size_t>::max() - digit) / 10) {
            return false;  // 溢出
        }
        value = value * 10 + digit;
    }
    out = value;
    return true;
}

std::vector<std::string> SplitWhitespace(const std::string& text) {
    std::vector<std::string> tokens;
    std::size_t index = 0;
    while (index < text.size()) {
        std::size_t width = 0;
        if (IsWhitespaceAt(text, index, width)) {
            index += width;
            continue;
        }
        const std::size_t start = index;
        while (index < text.size() && !IsWhitespaceAt(text, index, width)) {
            ++index;
        }
        tokens.push_back(text.substr(start, index - start));
    }
    return tokens;
}

std::string Join(const std::vector<std::string>& parts, const std::string& separator) {
    std::string result;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) {
            result += separator;
        }
        result += parts[i];
    }
    return result;
}

bool IsAsciiAlpha(char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

}  // namespace zuma::util
