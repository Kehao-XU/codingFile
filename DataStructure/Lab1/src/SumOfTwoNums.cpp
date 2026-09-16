#include <iostream>
#include <unordered_map>
#include <vector>

/*
给定一个整数数组 nums 和一个整数目标值
target，请你在该数组中找出和为目标值target的那两个整数，并返回它们的数组下标。
假设每种输入只会对应一个答案，并且你不能使用两次相同的元素。

思路（哈希表，一次遍历 O(n)）：
    对每个 nums[i]，先算补数 need = target - nums[i]；
    若 need 已经在哈希表里（说明前面出现过），直接返回 {表里 need 的下标, i}；
    否则把 nums[i] -> i 存进表，继续。
"不能使用两次相同的元素" 靠"先查后存"保证：查的时候表里只有 i 之前的元素，
所以不会把 nums[i] 自己配对两次。
*/
std::vector<int> solution(std::vector<int> nums, int target) {
    std::unordered_map<int, int> seen; // 值 -> 下标
    for (int i = 0; i < (int)nums.size(); ++i) {
        int need = target - nums[i];
        auto it = seen.find(need);
        if (it != seen.end()) {
            return {it->second, i}; // 找到：返回两个下标
        }
        seen[nums[i]] = i; // 先查后存，避免重复用同一元素
    }
    return {}; // 题目保证有解，这里只是兜底
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 10, 23, 15};
    std::vector<int> ans = solution(nums, 9);
    std::cout << ans[0] << ',' << ans[1] << '\n';
    return 0;
}