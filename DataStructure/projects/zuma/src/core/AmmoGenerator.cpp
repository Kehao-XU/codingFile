#include "core/AmmoGenerator.h"

namespace zuma::core {

AmmoGenerator::AmmoGenerator(const std::string& pattern) : pattern_(pattern) {
    if (pattern_.empty()) {
        pattern_ = "?";  // 兜底：关卡校验会拦截空 AMMO，这里保证不会越界
    }
    const std::size_t length = pattern_.size();
    current_ = pattern_[0];
    next_ = length > 1 ? pattern_[1] : pattern_[0];
    cursor_ = (length > 1 ? 2 : 1) % length;
}

void AmmoGenerator::Advance() {
    current_ = next_;
    next_ = pattern_[cursor_];
    cursor_ = (cursor_ + 1) % pattern_.size();
}

void AmmoGenerator::Restore(std::size_t cursor, char current, char next) {
    cursor_ = cursor % pattern_.size();
    current_ = current;
    next_ = next;
}

}  // namespace zuma::core
