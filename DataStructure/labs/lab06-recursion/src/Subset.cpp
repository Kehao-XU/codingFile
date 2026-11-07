/**
 * 给定一个不含重复元素的整数数组
nums。编写递归回溯算法，生成数组的全部子集（幂集）。子集不能包含重复集合，输出可以按任意顺序。
只能使用递归回溯实现，禁止位运算、循环迭代生成子集等非递归捷径；
说明递归函数各个参数含义，写出递归终止条件；
说明回溯步骤中 “选择元素” 和 “撤销选择” 分别做了什么；
分析时间复杂度与递归工作栈的空间复杂度。
 */

#include <algorithm>
#include <iostream>
#include <vector>

// 递归回溯生成全部子集（幂集），只用递归，不用位运算/迭代枚举。
//
// 参数含义：
//   nums  : 原始数组（只读，不含重复元素）
//   i     : 当前正在决策 nums[i] 是否加入当前子集
//   path  : 当前已经选出的元素，即正在构造的那个子集
//   ans   : 结果集合，收集所有完整子集
//
// 终止条件：i == nums.size()，即每个元素都已做过“选/不选”的决策，
//           此时把 path 作为一整个子集记录进 ans，然后返回。
//
// 回溯过程（二叉决策树，左分支不选、右分支选）：
//   “选择元素”：path.push_back(nums[i])，把 nums[i] 放进当前子集；
//   “撤销选择”：path.pop_back()，回到本层时把刚才放进去的元素取出来，
//              让 path 恢复成进入本层之前的模样，好让同一层的其他分支复用。
static void dfs(const std::vector<int> &nums, int i, std::vector<int> &path,
                std::vector<std::vector<int>> &ans) {
    if (i == (int)nums.size()) { // 终止条件：所有元素决策完毕
        ans.push_back(path);
        return;
    }

    dfs(nums, i + 1, path, ans); // 分支一：不选 nums[i]

    path.push_back(nums[i]);     // 分支二：选 nums[i]
    dfs(nums, i + 1, path, ans); // 带着 nums[i] 继续决策后面的元素
    path.pop_back();             // 撤销选择：回溯，恢复现场

    // 复杂度：每个元素 2 种决策，共 2^n 个叶子/子集，每个子集拷贝进 ans 需
    // O(n)，故时间 O(n * 2^n)；递归工作栈深度为 n，额外空间 O(n)
    // （不含存放结果的 O(n * 2^n)）。
}

std::vector<std::vector<int>> solution(std::vector<int> nums) {
    std::vector<std::vector<int>> ans;
    std::vector<int> path;
    dfs(nums, 0, path, ans);
    return ans;
}

// 打印一个集合，如 {1, 2, 3}；空集打印 {}
static void printSubset(const std::vector<int> &subset) {
    std::cout << '{';
    for (size_t i = 0; i < subset.size(); ++i) {
        if (i > 0)
            std::cout << ", ";
        std::cout << subset[i];
    }
    std::cout << '}';
}

int main() {
    std::vector<int> nums;
    std::cout << "Please input nums" << '\n';
    int x;
    while (std::cin >> x) {
        nums.push_back(x);
    }

    std::cout << "nums = ";
    printSubset(nums);
    std::cout << '\n';

    std::vector<std::vector<int>> ans = solution(nums);

    // 题目允许子集按任意顺序输出，这里只是为了显示整齐：
    // 先把每个子集内部升序排列，再把所有子集按字典序排列。
    for (size_t i = 0; i < ans.size(); ++i) {
        std::sort(ans[i].begin(), ans[i].end());
    }
    std::sort(ans.begin(), ans.end());

    // 理论子集数 2^n（n = nums.size()），用循环计算，不写位运算
    size_t expected = 1;
    for (size_t i = 0; i < nums.size(); ++i) {
        expected *= 2;
    }

    std::cout << "subsets (" << ans.size() << ", expect " << expected << "):\n";
    for (size_t i = 0; i < ans.size(); ++i) {
        std::cout << "  ";
        printSubset(ans[i]);
        std::cout << '\n';
    }

    // 自检 1：排序后相邻元素相同 => 存在重复子集
    bool hasDuplicate = std::adjacent_find(ans.begin(), ans.end()) != ans.end();
    // 自检 2：子集个数应为 2^n
    bool countOk = (ans.size() == expected);

    if (hasDuplicate)
        std::cout << "[FAIL] duplicate subsets found\n";
    if (!countOk)
        std::cout << "[FAIL] subset count " << ans.size()
                  << " != 2^n = " << expected << '\n';
    if (!hasDuplicate && countOk)
        std::cout << "[OK] no duplicate subset, count == 2^n\n";

    return 0;
}