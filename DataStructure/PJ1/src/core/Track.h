#pragma once

#include <cstddef>
#include <list>

#include "core/Ball.h"

namespace zuma::core {

// 轨道球链。底层为双向链表（std::list），见架构文档 DS1：
// 任意位置插入 O(1)、区间删除一次完成、且不会使其它节点的迭代器失效，
// 便于消除/连锁阶段持有断点两侧的迭代器。
class Track {
public:
    using Container = std::list<Ball>;
    using iterator = Container::iterator;
    using const_iterator = Container::const_iterator;

    bool empty() const noexcept { return balls_.empty(); }
    std::size_t size() const noexcept { return balls_.size(); }

    iterator begin() noexcept { return balls_.begin(); }
    iterator end() noexcept { return balls_.end(); }
    const_iterator begin() const noexcept { return balls_.begin(); }
    const_iterator end() const noexcept { return balls_.end(); }

    // 1 基位置 -> 迭代器；pos 超出 size()+1 时返回 end()。
    iterator PositionOf(std::size_t position);
    const_iterator PositionOf(std::size_t position) const;

    // 在位置 position 之前插入；调用方须保证 1 <= position <= size()+1。
    // 返回新球的迭代器。
    iterator Insert(std::size_t position, Ball ball);

    // 追加到入口端（等价于 Insert(size()+1, ball)）。
    iterator Append(Ball ball);

    // 删除 [first, last)，返回 last（区间之后的首个节点）。
    iterator Erase(iterator first, iterator last);

    void Clear() noexcept { balls_.clear(); }

    // 是否存在长度 >= 3 的同色连续段（关卡加载校验用）。
    bool HasRunOfThreeOrMore() const;

private:
    Container balls_;
};

}  // namespace zuma::core
