// lab9-02.cpp — 二叉搜索树中第 k 小的元素
//
// 题目：给定 BST 的根结点 root 和整数 k（k 从 1 开始），返回其中第 k 小的值。
//
// 思路：BST 的中序遍历就是升序序列，所以中序遍历数到第 k 个即可。
//       这里用显式栈写非递归版本：一路把左孩子压栈，弹出时计数，
//       数到 k 直接返回，不用把整棵树走完；随后转向右子树继续同样的过程。
//
// 复杂度：时间 O(h + k)，h 为树高；空间 O(h)，只存一条左链。
//
// 测试用例（对应 main 里的 4 组）：
//   1) root = [3,1,4,null,2], k = 1 → 1
//   2) 同一棵树, k = 2 → 2
//   3) 同一棵树, k = 3 → 3
//   4) 同一棵树, k = 4 → 4

#include <climits>
#include <cstdio>
#include <queue>
#include <stack>
#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    explicit TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

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

// 核心解法：中序遍历的第 k 个结点
int kthSmallest(TreeNode *root, int k) {
    std::stack<TreeNode *> st;
    TreeNode *p = root;
    while (p || !st.empty()) {
        while (p) {          // 一路向左，压栈
            st.push(p);
            p = p->left;
        }
        p = st.top();
        st.pop();
        if (--k == 0) return p->val;   // 第 k 个
        p = p->right;                  // 转向右子树
    }
    return -1;   // k 超出结点数，按题目保证不会走到这里
}

static void check(int id, const std::vector<int> &a, int k, int expect) {
    TreeNode *root = buildTree(a);
    int got = kthSmallest(root, k);
    std::printf("用例 %d: k=%d  得到 %d，期望 %d  %s\n",
                id, k, got, expect, got == expect ? "OK" : "FAIL");
    destroy(root);
}

int main() {
    const std::vector<int> tree = {3, 1, 4, INT_MIN, 2};
    check(1, tree, 1, 1);
    check(2, tree, 2, 2);
    check(3, tree, 3, 3);
    check(4, tree, 4, 4);
    return 0;
}
