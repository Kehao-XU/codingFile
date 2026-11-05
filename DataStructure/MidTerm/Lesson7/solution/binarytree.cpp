// Lesson7/src/binarytree.cpp
// 由前序+中序建树、三种遍历、统计量、层序判完全二叉树。
// 对应学案 07 第九节。
// 构建：g++ -O2 -std=c++17 src/binarytree.cpp -o output/binarytree.exe

#include <cstdio>
#include <queue>

struct Node {
    char data;
    Node *left, *right;
    Node(char d) : data(d), left(nullptr), right(nullptr) {}
};

// ---------- 由前序 + 中序建树 ----------
// vlr: 前序序列起点；lvr: 中序序列起点；n: 本子树结点数
Node *buildFromPreIn(const char *vlr, const char *lvr, int n) {
    if (n == 0) return nullptr;
    char root = vlr[0];
    int k = 0;
    while (lvr[k] != root) ++k;          // 根在中序中的位置 = 左子树大小
    Node *p = new Node(root);
    p->left = buildFromPreIn(vlr + 1, lvr, k);
    p->right = buildFromPreIn(vlr + k + 1, lvr + k + 1, n - k - 1);
    return p;
}

// ---------- 遍历 ----------
void preOrder(Node *p) {
    if (!p) return;
    std::printf("%c", p->data);
    preOrder(p->left);
    preOrder(p->right);
}

void inOrder(Node *p) {
    if (!p) return;
    inOrder(p->left);
    std::printf("%c", p->data);
    inOrder(p->right);
}

void postOrder(Node *p) {
    if (!p) return;
    postOrder(p->left);
    postOrder(p->right);
    std::printf("%c", p->data);
}

void levelOrder(Node *p) {
    if (!p) return;
    std::queue<Node *> q;
    q.push(p);
    while (!q.empty()) {
        Node *cur = q.front();
        q.pop();
        std::printf("%c", cur->data);
        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
}

// ---------- 统计量 ----------
int size(Node *p) { return p ? 1 + size(p->left) + size(p->right) : 0; }

int height(Node *p) {
    if (!p) return 0;
    int l = height(p->left), r = height(p->right);
    return 1 + (l > r ? l : r);
}

int leaves(Node *p) {
    if (!p) return 0;
    if (!p->left && !p->right) return 1;
    return leaves(p->left) + leaves(p->right);
}

int emptyPointers(Node *p) {                       // 空指针域个数
    if (!p) return 0;
    int self = (p->left ? 0 : 1) + (p->right ? 0 : 1);
    return self + emptyPointers(p->left) + emptyPointers(p->right);
}

// ---------- 判完全二叉树：层序，空子女也入队 ----------
bool isComplete(Node *root) {
    if (!root) return true;
    std::queue<Node *> q;
    q.push(root);
    bool seenNull = false;
    while (!q.empty()) {
        Node *p = q.front();
        q.pop();
        if (!p) {
            seenNull = true;
            continue;
        }
        if (seenNull) return false;                // 空结点之后又出现非空
        q.push(p->left);
        q.push(p->right);
    }
    return true;
}

void destroy(Node *p) {
    if (!p) return;
    destroy(p->left);
    destroy(p->right);
    delete p;
}

int main() {
    // 1. 课件那组序列
    const char *vlr = "ABHFDECKG";
    const char *lvr = "HBDFAEKCG";
    Node *root = buildFromPreIn(vlr, lvr, 9);

    std::printf("=== 任务 1：由前序 + 中序建树 ===\n");
    std::printf("输入前序：%s\n", vlr);
    std::printf("输入中序：%s\n", lvr);
    std::printf("前序："); preOrder(root);  std::printf("\n");
    std::printf("中序："); inOrder(root);   std::printf("\n");
    std::printf("后序："); postOrder(root); std::printf("\n");
    std::printf("层序："); levelOrder(root); std::printf("\n\n");

    std::printf("=== 任务 2：统计量 ===\n");
    std::printf("结点数 %d，高度 %d，叶子数 %d，空指针域 %d\n\n",
                size(root), height(root), leaves(root), emptyPointers(root));

    std::printf("=== 任务 3：判完全二叉树 ===\n");
    std::printf("上面那棵：%s\n", isComplete(root) ? "是完全二叉树" : "不是完全二叉树");

    // 按层序数组建一棵完全二叉树：下标 i 的左子女 2i+1，右子女 2i+2
    const char *arr = "1234567890";
    Node *nodes[10];
    for (int i = 0; i < 10; ++i) nodes[i] = new Node(arr[i]);
    for (int i = 0; i < 10; ++i) {
        int l = 2 * i + 1, r = 2 * i + 2;
        nodes[i]->left = (l < 10) ? nodes[l] : nullptr;
        nodes[i]->right = (r < 10) ? nodes[r] : nullptr;
    }
    std::printf("层序数组 1234567890 建的树：%s\n", isComplete(nodes[0]) ? "完全二叉树" : "不是");
    std::printf("层序："); levelOrder(nodes[0]); std::printf("\n\n");

    std::printf("=== 任务 4：两种建法的对比 ===\n");
    std::printf("按完全二叉树编号存数组，高度 %d 最坏要 2^h-1 = %d 个位置；\n",
                height(root), (1 << height(root)) - 1);
    std::printf("二叉链表只要 %d 个结点，每个结点 2 个指针。\n", size(root));

    destroy(root);
    destroy(nodes[0]);
    return 0;
}
