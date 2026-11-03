#include "core/FeedQueue.h"

namespace zuma::core {

FeedQueue::FeedQueue(const std::string& feed) {
    for (char color : feed) {
        queue_.push_back(Ball{color});
    }
}

std::optional<Ball> FeedQueue::Pop() {
    if (queue_.empty()) {
        return std::nullopt;
    }
    const Ball front = queue_.front();
    queue_.pop_front();
    return front;
}

std::vector<char> FeedQueue::Snapshot() const {
    std::vector<char> colors;
    colors.reserve(queue_.size());
    for (const Ball& ball : queue_) {
        colors.push_back(ball.color);
    }
    return colors;
}

void FeedQueue::Restore(const std::vector<char>& colors) {
    queue_.clear();
    for (char color : colors) {
        queue_.push_back(Ball{color});
    }
}

}  // namespace zuma::core
