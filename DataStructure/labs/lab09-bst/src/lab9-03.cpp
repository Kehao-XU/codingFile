// lab9-03.cpp — 二叉搜索树迭代器
//
// 题目：实现 BSTIterator，按中序遍历的顺序逐个返回值。
//   BSTIterator(TreeNode *root)  构造，指针停在第一个元素之前
//   int  next()                  返回下一个最小值
//   bool hasNext()               是否还有下一个
//
// 思路：本质是把「递归版中序遍历」的调用栈手动保存下来。
//       构造函数里沿最左链一路压栈，栈顶就是当前最小值。
//       next() 弹出栈顶作为答案，然后把这个结点的右子树的最左链压进去。
//       这样每个结点只入栈出栈一次。
//
// 复杂度：next() 和 hasNext() 均摊 O(1)（每个结点总共进出栈一次），空间 O(h)。
//
// 测试用例（对应 main 里的 3 组）：
//   1) root = [7,3,15,null,null,9,20]
//      依次 next → 3, 7, 9, 15, 20，最后 hasNext = false
//   2) root = [1]        → next = 1，hasNext = false
//   3) root = [2,1,3]    → 1, 2, 3

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

class BSTIterator {
public:
    explicit BSTIterator(TreeNode *root) { pushLeftChain(root); }

    int next() {
        TreeNode *p = st_.top();
        st_.pop();
        pushLeftChain(p->right);   // 右子树的最小值接在后面
        return p->val;
    }

    bool hasNext() const { return !st_.empty(); }

private:
    std::stack<TreeNode *> st_;

    void pushLeftChain(TreeNode *p) {
        while (p) {
            st_.push(p);
            p = p->left;
        }
    }
};

static void run(int id, const std::vector<int> &a) {
    TreeNode *root = buildTree(a);
    BSTIterator it(root);
    std::printf("用例 %d: ", id);
    while (it.hasNext()) std::printf("%d ", it.next());
    std::printf("\n");
    destroy(root);
}

int main() {
    run(1, {7, 3, 15, INT_MIN, INT_MIN, 9, 20});   // 期望 3 7 9 15 20
    run(2, {1});                                    // 期望 1
    run(3, {2, 1, 3});                              // 期望 1 2 3
    return 0;
}
