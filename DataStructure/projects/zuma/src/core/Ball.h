#pragma once

namespace zuma::core {

// 一个彩球。当前只携带颜色，后续扩展（如炸弹球）可在此追加字段。
struct Ball {
    char color = '?';
};

inline bool operator==(const Ball& lhs, const Ball& rhs) {
    return lhs.color == rhs.color;
}

inline bool operator!=(const Ball& lhs, const Ball& rhs) {
    return !(lhs == rhs);
}

}  // namespace zuma::core
