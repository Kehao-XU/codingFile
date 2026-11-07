#include <iostream>
#include <vector>

void solution(const std::vector<int> &nums) {
    int maxlen = 1;
    int l = 0, r = 1, maxL = 0, maxR = 1, len;
    int SIZE = nums.size();

    while (l < SIZE - maxlen) {
        while (r < SIZE && nums[r] > nums[r - 1])
            ++r;
        len = r - l;
        if (len > maxlen) {
            maxL = l;
            maxR = r;
            maxlen = len;
        }
        l = r++;
    }

    std::cout << '[';
    for (int i = maxL; i < maxR - 1; ++i) {
        std::cout << nums[i] << ',';
    }
    std::cout << nums[maxR - 1] << ']' << '\n' << maxlen << '\n';
}

int main() {
    std::vector<int> nums = {1, 3, 5, 4, 7};
    solution(nums);
    return 0;
}