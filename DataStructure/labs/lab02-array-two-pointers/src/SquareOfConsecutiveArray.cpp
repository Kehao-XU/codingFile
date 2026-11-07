/**
 * 给你一个按 非递减顺序 排序的整数数组 nums，返回 每个数字的平方
组成的新数组，要求也按 非递减顺序 排序。 注： 1 <= s.length <= 105。

示例：
输入:nums = [-4,-1,0,3,10]
输出: [0,1,9,16,100]
 */

#include <iostream>
#include <stack>
#include <vector>

std::vector<int> solution(const std::vector<int> &nums) {
    if (nums.empty()) {
        return {};
    }

    size_t SIZE = nums.size();
    size_t i = SIZE - 1;
    std::vector<int> ans(SIZE);
    auto l = nums.begin(), r = nums.end() - 1;

    while (l <= r) {
        int sql = *l * *l, sqr = *r * *r;
        if (sql < sqr) {
            ans[i--] = sqr;
            --r;
        } else {
            ans[i--] = sql;
            ++l;
        }
    }
    return ans;
}

int main() {
    std::vector<int> nums = {-4, -1, 0, 3, 10}, ans_nums;
    ans_nums = solution(nums);

    std::cout << '[';
    for (int i = 0; i < (int)nums.size() - 1; ++i) {
        std::cout << ans_nums[i] << ',';
    }
    std::cout << ans_nums[(int)nums.size() - 1] << ']' << '\n';

    return 0;
}