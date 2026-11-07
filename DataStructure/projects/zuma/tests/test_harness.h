#pragma once

// 极简测试框架（不依赖任何第三方库）：CHECK / CHECK_EQ / CHECK_FALSE + CASE 分组。
// 仅使用 C++17 的内联变量与模板。

#include <cstddef>
#include <cstdio>
#include <string>
#include <type_traits>
#include <vector>

namespace zuma::test {

inline int g_checks = 0;
inline int g_failures = 0;
inline std::string g_caseName;

inline void BeginCase(const std::string& name) {
    g_caseName = name;
}

inline void Fail(const std::string& message, const char* file, int line) {
    ++g_failures;
    std::printf("  [FAIL] %s\n         %s:%d\n         %s\n", g_caseName.c_str(), file, line,
                message.c_str());
}

inline std::string ToStr(bool value) {
    return value ? "true" : "false";
}
inline std::string ToStr(char value) {
    return std::string("'") + value + "'";
}
inline std::string ToStr(const std::string& value) {
    return "\"" + value + "\"";
}
inline std::string ToStr(const char* value) {
    return std::string("\"") + (value == nullptr ? "(null)" : value) + "\"";
}

template <typename T,
          typename = std::enable_if_t<std::is_integral<T>::value && !std::is_same<T, bool>::value &&
                                      !std::is_same<T, char>::value>>
std::string ToStr(T value) {
    return std::to_string(value);
}

template <typename T>
std::string ToStr(const std::vector<T>& values) {
    std::string text = "[";
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index != 0) {
            text += ", ";
        }
        text += ToStr(values[index]);
    }
    return text + "]";
}

template <typename A, typename B>
void CheckEq(const A& actual, const B& expected, const char* exprActual, const char* exprExpected,
             const char* file, int line) {
    ++g_checks;
    using Common = std::common_type_t<A, B>;
    if (!(static_cast<Common>(actual) == static_cast<Common>(expected))) {
        Fail(std::string("CHECK_EQ 失败：") + exprActual + " = " + ToStr(actual) + "，但 " +
                 exprExpected + " = " + ToStr(expected),
             file, line);
    }
}

inline void CheckTrue(bool value, const char* expr, const char* file, int line) {
    ++g_checks;
    if (!value) {
        Fail(std::string("CHECK(") + expr + ") 为 false", file, line);
    }
}

inline void CheckFalse(bool value, const char* expr, const char* file, int line) {
    ++g_checks;
    if (value) {
        Fail(std::string("CHECK_FALSE(") + expr + ") 为 true", file, line);
    }
}

}  // namespace zuma::test

#define CASE(name) ::zuma::test::BeginCase(name)
#define CHECK(expr) ::zuma::test::CheckTrue(static_cast<bool>(expr), #expr, __FILE__, __LINE__)
#define CHECK_FALSE(expr) ::zuma::test::CheckFalse(static_cast<bool>(expr), #expr, __FILE__, __LINE__)
#define CHECK_EQ(actual, expected) \
    ::zuma::test::CheckEq((actual), (expected), #actual, #expected, __FILE__, __LINE__)
