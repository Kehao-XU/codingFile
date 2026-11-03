/**
 * 给定一个整数数组nums，以及一个大小为k的滑动窗口，从数组的最左侧移动到最右侧。
 * 你只能看到窗口k里的数字。
 * 每次滑动窗口都会向右移动一个位置。返回每次窗口中的最大值。
 */

#include <iostream>
#include <list>
#include <vector>

std::vector<int> solution(std::vector<int> nums, int k) {
    if (k <= 0 || k > (int)nums.size())
        return {};

    std::vector<int> ans;
    std::list<int> window;  // values in the window, kept non-increasing

    for (int i = 0; i < (int)nums.size(); ++i) {
        if (i >= k) {  // remove the element that just left the window
            auto it = window.begin();
            while (*it != nums[i - k])
                ++it;
            window.erase(it);
        }

        auto it = window.begin();  // insert and keep the list non-increasing
        while (it != window.end() && nums[i] < *it)
            ++it;
        window.insert(it, nums[i]);

        if (i >= k - 1)  // the window is full
            ans.push_back(window.front());
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
