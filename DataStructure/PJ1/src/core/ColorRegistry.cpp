#include "core/ColorRegistry.h"

#include <algorithm>

namespace zuma::core {

ColorRegistry::ColorRegistry() {
    table_ = {
        {'R', {'R', "红", "\x1b[31m"}},
        {'G', {'G', "绿", "\x1b[32m"}},
        {'B', {'B', "蓝", "\x1b[34m"}},
        {'Y', {'Y', "黄", "\x1b[33m"}},
        {'P', {'P', "紫", "\x1b[35m"}},
        {'C', {'C', "青", "\x1b[36m"}},
        {'O', {'O', "橙", "\x1b[38;5;208m"}},
        {'W', {'W', "白", "\x1b[37m"}},
        {'K', {'K', "灰", "\x1b[90m"}},
    };
}

bool ColorRegistry::Contains(char key) const {
    return table_.find(key) != table_.end();
}

const ColorInfo& ColorRegistry::Get(char key) const {
    const auto it = table_.find(key);
    return it == table_.end() ? Fallback() : it->second;
}

const ColorInfo& ColorRegistry::Fallback() {
    static const ColorInfo fallback{'?', "未注册", ""};
    return fallback;
}

std::vector<char> ColorRegistry::Keys() const {
    std::vector<char> keys;
    keys.reserve(table_.size());
    for (const auto& entry : table_) {
        keys.push_back(entry.first);
    }
    std::sort(keys.begin(), keys.end());
    return keys;
}

}  // namespace zuma::core
