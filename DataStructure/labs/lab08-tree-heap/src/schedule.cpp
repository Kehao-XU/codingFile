/**
 * 某系统需要管理一组定时事件，每个事件具有唯一的触发时间。请使用二叉搜索树存储事件，并将其转换为中序线索二叉树，实现指定时间区间内的事件查询。
要求：
根据给定序列建立二叉搜索树，并进行中序线索化。
给定时间区间 [L, R]，输出所有触发时间位于该区间内的事件，按时间升序排列。
查询时利用中序后继线索进行遍历，不允许使用递归、辅助栈或额外的有序数组。
示例：
输入：
events = [20,10,30,5,15,25,35]
L = 9, R = 26
输出：
[10,15,20,25]
说明： 二叉搜索树的中序序列为 [5,10,15,20,25,30,35]，区间 [9,26] 内的事件为
[10,15,20,25]。

 */

#include <cstdio>
#include <vector>

struct Node {
    int ts;
    Node *left;
    Node *right;
    bool rTag; // true 表示 right 存的是中序后继线索
};

// 插入二叉搜索树（时间戳唯一，重复则忽略）
static Node *insert(Node *root, int ts) {
    if (!root)
        return new Node{ts, nullptr, nullptr, false};
    Node *cur = root;
    while (true) {
        if (ts < cur->ts) {
            if (cur->left)
                cur = cur->left;
            else {
                cur->left = new Node{ts, nullptr, nullptr, false};
                break;
            }
        } else if (ts > cur->ts) {
            if (cur->right)
                cur = cur->right;
            else {
                cur->right = new Node{ts, nullptr, nullptr, false};
                break;
            }
        } else {
            break;
        }
    }
    return root;
}

// 中序线索化（建表阶段，允许递归；查询阶段不许用）
static void thread(Node *cur, Node *&prev) {
    if (!cur)
        return;
    thread(cur->left, prev);
    if (prev && prev->right == nullptr) {
        prev->right = cur; // 右链域改成后继线索
        prev->rTag = true;
    }
    prev = cur;
    thread(cur->right, prev);
}

// 当前结点的中序后继：有线索就走线索，否则走右子树最左端
static Node *succ(Node *n) {
    if (n->rTag)
        return n->right;
    Node *p = n->right;
    if (!p)
        return nullptr;
    while (p->left)
        p = p->left;
    return p;
}

// 在普通二叉搜索树上找第一个 >= L 的结点（此时代码还没线索化）
static Node *lowerBound(Node *root, int L) {
    Node *res = nullptr, *cur = root;
    while (cur) {
        if (cur->ts >= L) {
            res = cur;
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return res;
}

// 区间查询：全程只靠后继线索，不用递归、栈或辅助数组
static std::vector<int> rangeQuery(Node *root, int L, int R) {
    Node *start = lowerBound(root, L); // 先定位起点
    Node *prev = nullptr;
    thread(root, prev);                // 再线索化

    std::vector<int> out;
    for (Node *p = start; p && p->ts <= R; p = succ(p))
        out.push_back(p->ts);
    return out;
}

int main() {
    const int events[] = {20, 10, 30, 5, 15, 25, 35};
    Node *root = nullptr;
    for (int x : events)
        root = insert(root, x);

    std::vector<int> res = rangeQuery(root, 9, 26);
    std::printf("[");
    for (int i = 0; i < (int)res.size(); ++i)
        std::printf("%s%d", i ? "," : "", res[i]);
    std::printf("]\n");
    return 0;
}