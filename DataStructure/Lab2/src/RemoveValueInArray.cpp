/**
 * 给你一个数组 nums?和一个值 val，你需要 原地
移除所有数值等于?val?的元素，并返回移除后数组的新长度。
不要使用额外的数组空间，你必须仅使用 O(1) 额外空间并原地修改输入数组。
元素的顺序可以改变。你不需要考虑数组中超出新长度后面的元素。

示例：
输入：给定 nums = [3,2,2,3], val = 3
输出：函数应该返回新的长度 2, 并且 nums 中的前两个元素均为 2
 */

#include <iostream>
#include <vector>

int solution(std::vector<int> &nums, int val) {
    auto i = nums.begin(), j = nums.begin();
    int count = 0;
    while (j != nums.end()) {
        if (*j != val) {
            *i = *j;
            ++i;
            ++count;
        }
        ++j;
    }
    return count;
}

int main() {
    std::vector<int> nums = {
        3, 2, 2, 3, 5, 6, 9, 10,
        3, 4, 3, 2, 2, 7, 8}; // 根据ppt中的测试用例编写nums数组
    int ans = solution(nums, 3);

    std::cout << ans << '\n';
    for (int i = 0; i < ans; ++i) {
        std::cout << nums[i] << ' ';
    }
    std::cout << '\n'; // 一行输出修改后nums的长度，另一行输出修改后的nums数组

    return 0;
}