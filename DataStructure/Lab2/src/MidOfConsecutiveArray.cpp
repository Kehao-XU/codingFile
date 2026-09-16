/**
 * 一个长度为L(L≥1）的升序序列S，处在第???/2?个位置的数称为S的中位数。例如,若序列S1=(11,13,15,17,19)，则S1的中位数是15,两个序列的中位数是含它们所有元素的升序序列的中位数。
 * 例如，若S2=(2,4,6,8,20)，则S1和S2的中位数是11。现在有两个等长升序序列A和B,试设计一个在时间和空间两方面都尽可能、高效的算法,找出两个序列A和B的中位数。
示例：
输入：nums1 = [11,13,15,17,19]  nums2 = [2,4,6,8,20]
输出：11
 */

#include <cmath>
#include <iostream>
#include <vector>

int solution(const std::vector<int> &nums1, const std::vector<int> &nums2) {
    size_t SIZE = nums1.size();
    int i = 0, j = 0, cnt = 0, mid = 0;

    while (cnt < SIZE) {
        if (nums1[i] < nums2[j]) {
            mid = nums1[i++];
            ++cnt;
        } else {
            mid = nums2[j++];
            ++cnt;
        }
    }

    return mid;
}

/**
 * Binary search version.
 *
 * nums1 and nums2 are ascending and have the same length n, so the answer is
 * simply the n-th smallest element of the two arrays together.
 *
 * Idea: keep a window [lo1, hi1] in nums1 and [lo2, hi2] in nums2. Both windows
 * always have the same size. Compare their middle elements:
 *   - equal: that value is the answer.
 *   - nums1[mid1] < nums2[mid2]:
 *       the left half of nums1 (small values) and the right half of nums2
 *       (large values) are both far away from the middle, so we may drop the
 *       same amount from each side and the median stays the same.
 *       odd window size : mid1 and mid2 may still be the answer, keep them.
 *       even window size: mid1 and mid2 can be dropped as well.
 *   - nums1[mid1] > nums2[mid2]: symmetric case.
 * Every round halves both windows -> Time O(log n), Space O(1).
 */
int solutionBinarySearch(const std::vector<int> &nums1,
                         const std::vector<int> &nums2) {
    int lo1 = 0, hi1 = (int)nums1.size() - 1;
    int lo2 = 0, hi2 = (int)nums2.size() - 1;

    while (lo1 != hi1 || lo2 != hi2) {
        int mid1 = lo1 + (hi1 - lo1) / 2;
        int mid2 = lo2 + (hi2 - lo2) / 2;

        if (nums1[mid1] == nums2[mid2]) {
            return nums1[mid1];
        }

        if (nums1[mid1] < nums2[mid2]) {
            if ((hi1 - lo1) % 2 == 0) { // odd window size: keep mid1 and mid2
                lo1 = mid1;
                hi2 = mid2;
            } else { // even window size: mid1 and mid2 can be dropped
                lo1 = mid1 + 1;
                hi2 = mid2;
            }
        } else {
            if ((hi1 - lo1) % 2 == 0) { // odd window size: keep mid1 and mid2
                hi1 = mid1;
                lo2 = mid2;
            } else { // even window size: mid1 and mid2 can be dropped
                hi1 = mid1;
                lo2 = mid2 + 1;
            }
        }
    }

    // Left with one element in each window: they are the two middle elements of
    // the merged sequence, the n-th one is the smaller of them.
    return nums1[lo1] < nums2[lo2] ? nums1[lo1] : nums2[lo2];
}

int main() {
    std::vector<int> nums1 = {11, 13, 15, 17, 19}, nums2 = {2, 4, 6, 8, 20};
    std::cout << "merge  : " << solution(nums1, nums2) << '\n';
    std::cout << "binary : " << solutionBinarySearch(nums1, nums2) << '\n';
    return 0;
}