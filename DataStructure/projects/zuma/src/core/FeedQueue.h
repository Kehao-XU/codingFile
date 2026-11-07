#pragma once

#include <cstddef>
#include <deque>
#include <optional>
#include <string>
#include <vector>

#include "core/Ball.h"

namespace zuma::core {

// 等待进入轨道的彩球（队列，FIFO）。底层容器用 deque 而非 std::queue，
// 是为了能只读遍历尚未入轨的彩球以完成显示；对外只暴露队列语义。
class FeedQueue {
public:
    FeedQueue() = default;
    explicit FeedQueue(const std::string& feed);

    bool empty() const noexcept { return queue_.empty(); }
    std::size_t size() const noexcept { return queue_.size(); }

    // 弹出队首彩球；队列为空时返回 std::nullopt（对应"等待区为空时不再加入"）。
    std::optional<Ball> Pop();

    // 按队首在前的顺序导出/恢复，供状态快照（撤销）使用。
    std::vector<char> Snapshot() const;
    void Restore(const std::vector<char>& colors);

    const std::deque<Ball>& Remaining() const noexcept { return queue_; }

private:
    std::deque<Ball> queue_;
};

}  // namespace zuma::core
