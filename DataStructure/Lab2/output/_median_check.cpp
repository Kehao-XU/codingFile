// Temporary cross-check harness: binary search vs. brute force. ASCII only.
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

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
            if ((hi1 - lo1) % 2 == 0) {
                lo1 = mid1;
                hi2 = mid2;
            } else {
                lo1 = mid1 + 1;
                hi2 = mid2;
            }
        } else {
            if ((hi1 - lo1) % 2 == 0) {
                hi1 = mid1;
                lo2 = mid2;
            } else {
                hi1 = mid1;
                lo2 = mid2 + 1;
            }
        }
    }

    return nums1[lo1] < nums2[lo2] ? nums1[lo1] : nums2[lo2];
}

int brute(const std::vector<int> &nums1, const std::vector<int> &nums2) {
    std::vector<int> all(nums1);
    all.insert(all.end(), nums2.begin(), nums2.end());
    std::sort(all.begin(), all.end());
    return all[nums1.size() - 1]; // n-th smallest, 0-based index n - 1
}

int main() {
    std::mt19937 rng(12345);
    int bad = 0;

    for (int t = 0; t < 20000; ++t) {
        int n = (int)(rng() % 30) + 1;
        std::vector<int> a(n), b(n);
        std::uniform_int_distribution<int> val(-15, 15);
        for (int i = 0; i < n; ++i) {
            a[i] = val(rng);
            b[i] = val(rng);
        }
        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());

        int got = solutionBinarySearch(a, b);
        int want = brute(a, b);
        if (got != want) {
            if (++bad <= 5) {
                std::cout << "MISMATCH n=" << n << " got=" << got
                          << " want=" << want << "\nA:";
                for (int x : a)
                    std::cout << ' ' << x;
                std::cout << "\nB:";
                for (int x : b)
                    std::cout << ' ' << x;
                std::cout << '\n';
            }
        }
    }

    std::cout << (bad == 0 ? "ALL 20000 RANDOM CASES PASSED\n"
                           : "FAILURES ABOVE\n");
    return 0;
}
