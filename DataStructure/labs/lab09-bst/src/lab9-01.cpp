// lab9-01.cpp — 二叉搜索树的范围和
//
// 题目：给定二叉搜索树的根结点 root，返回值位于区间 [low, high] 内的所有结点值之和。
//
// 思路：利用 BST 的有序性剪枝，而不是遍历整棵树。
//   root->val < low   → 左子树全部小于 low，只看右子树
//   root->val > high  → 右子树全部大于 high，只看左子树
//   否则 root 落在区间内，答案加上自己，左右两边都要递归
//
// 复杂度：最坏 O(n)（树退化成链且区间覆盖全部）；平均只访问区间内的结点，
//         O(h + k)，h 为树高，k 为区间内结点数。
//
// 测试用例（对应 main 里的 4 组）：
//   1) root = [10,5,15,3,7,null,18], low = 7,  high = 15 → 32  （7+10+15）
//   2) root = [10,5,15,3,7,13,18,1,null,6], low = 6, high = 10 → 23  （6+7+10）
//   3) root = [5], low = 1, high = 10 → 5
//   4) root = [5], low = 6, high = 10 → 0

#include <climits>
#include <cstdio>
#include <queue>
#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    explicit TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// 按层序数组建树，INT_MIN 表示空位（和 LeetCode 的写法一致）
static TreeNode *buildTree(const std::vector<int> &a) {
    if (a.empty() || a[0] == INT_MIN) return nullptr;
    TreeNode *root = new TreeNode(a[0]);
    std::queue<TreeNode *> q;
    q.push(root);
    std::size_t i = 1;
    while (!q.empty() && i < a.size()) {
        TreeNode *p = q.front();
        q.pop();
        if (i < a.size() && a[i] != INT_MIN) {
            p->left = new TreeNode(a[i]);
            q.push(p->left);
        }
        ++i;
        if (i < a.size() && a[i] != INT_MIN) {
            p->right = new TreeNode(a[i]);
            q.push(p->right);
        }
        ++i;
    }
    return root;
}

static void destroy(TreeNode *p) {
    if (!p) return;
    destroy(p->left);
    destroy(p->right);
    delete p;
}

// 核心解法
int rangeSumBST(TreeNode *root, int low, int high) {
    if (!root) return 0;
    if (root->val < low) return rangeSumBST(root->right, low, high);
    if (root->val > high) return rangeSumBST(root->left, low, high);
    return root->val + rangeSumBST(root->left, low, high) +
           rangeSumBST(root->right, low, high);
}

static void check(int id, const std::vector<int> &a, int low, int high, int expect) {
    TreeNode *root = buildTree(a);
    int got = rangeSumBST(root, low, high);
    std::printf("用例 %d: low=%d high=%d  得到 %d，期望 %d  %s\n",
                id, low, high, got, expect, got == expect ? "OK" : "FAIL");
    destroy(root);
}

int main() {
    check(1, {10, 5, 15, 3, 7, INT_MIN, 18}, 7, 15, 32);
    check(2, {10, 5, 15, 3, 7, 13, 18, 1, INT_MIN, 6}, 6, 10, 23);
    check(3, {5}, 1, 10, 5);
    check(4, {5}, 6, 10, 0);
    return 0;
}
