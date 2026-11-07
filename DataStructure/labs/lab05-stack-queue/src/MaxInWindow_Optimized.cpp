/**
 * 给定一个整数数组nums，以及一个大小为k的滑动窗口，从数组的最左侧移动到最右侧。
 * 你只能看到窗口k里的数字。
 * 每次滑动窗口都会向右移动一个位置。返回每次窗口中的最大值。
 *
 * 这是 MaxInWindow.cpp 的 O(n) 版本，用 std::deque 实现单调队列。
 *
 * std::deque 是“双端队列”，两端增删都是 O(1)，常用接口：
 *   dq.push_back(x)   在队尾插入 x
 *   dq.pop_back()     弹出队尾元素
 *   dq.push_front(x)  在队首插入 x
 *   dq.pop_front()    弹出队首元素
 *   dq.front()/dq.back()  访问队首/队尾元素
 *   dq.empty()/dq.size()  判空/元素个数
 * 与 vector 的差别：vector 只能在尾部 O(1) 增删，而 deque 在头部也能 O(1)
 * 增删，所以“从队首删掉滑出窗口的元素”这个动作很自然。
 */

#include <deque>
#include <iostream>
#include <vector>

std::vector<int> solution(std::vector<int> nums, int k) {
    if (k <= 0 || k > (int)nums.size())
        return {};

    std::vector<int> ans;
    std::deque<int> window;  // 存下标，对应的数值从队首到队尾单调递减

    for (int i = 0; i < (int)nums.size(); ++i) {
        // 1) 队首下标已经滑出窗口 [i-k+1, i]，丢掉
        while (!window.empty() && window.front() <= i - k)
            window.pop_front();

        // 2) 新元素从队尾入队：尾部比它小（或相等）的元素永远不可能再当最大值
        while (!window.empty() && nums[window.back()] <= nums[i])
            window.pop_back();
        window.push_back(i);

        // 3) 窗口满了就取队首（它就是当前窗口的最大值）
        if (i >= k - 1)
            ans.push_back(nums[window.front()]);
    }

    return ans;
}

int main() {
    int n, k;
    if (!(std::cin >> n >> k))
        return 0;

    std::vector<int> nums(n);
    for (int i = 0; i < n; ++i)
        std::cin >> nums[i];

    std::vector<int> ans = solution(nums, k);
    for (int i = 0; i < (int)ans.size(); ++i)
        std::cout << ans[i] << (i + 1 == (int)ans.size() ? '\n' : ' ');

    return 0;
}
