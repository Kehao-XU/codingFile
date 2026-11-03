#pragma once

#include <cstddef>
#include <string>

namespace zuma::core {

// 弹药生成器：把 AMMO 字符串建模为循环缓冲（环形队列），
// 取到末位后取模回到首位，因此弹药无限、永不耗尽。
class AmmoGenerator {
public:
    explicit AmmoGenerator(const std::string& pattern);

    char Current() const noexcept { return current_; }
    char Next() const noexcept { return next_; }
    std::size_t Cursor() const noexcept { return cursor_; }
    const std::string& Pattern() const noexcept { return pattern_; }

    // 一个有效回合结束后推进：当前球 <- 下一个球，下一个球 <- 循环缓冲的下一个字符。
    void Advance();

    // 供状态快照（撤销）使用。
    void Restore(std::size_t cursor, char current, char next);

private:
    std::string pattern_;
    std::size_t cursor_ = 0;  // 下一个待取字符下标
    char current_ = '?';
    char next_ = '?';
};

}  // namespace zuma::core
