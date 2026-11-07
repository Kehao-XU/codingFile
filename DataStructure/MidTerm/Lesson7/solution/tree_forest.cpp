// Lesson7/src/tree_forest.cpp
// 补充练习：树、森林与二叉树的转换（左孩子-右兄弟）。
// 对应学案 07 第八节。
// 构建：g++ -O2 -std=c++17 src/tree_forest.cpp -o output/tree_forest.exe

#include <cstdio>
#include <queue>

struct TreeNode {
    char data;
    TreeNode *firstChild, *nextSibling;
    TreeNode(char d) : data(d), firstChild(nullptr), nextSibling(nullptr) {}
};

struct BinNode {
    char data;
    BinNode *left, *right;
    BinNode(char d) : data(d), left(nullptr), right(nullptr) {}
};

// ---------- 任务 1：建树 A(B(E,F),C,D(G)) ----------
TreeNode *buildSampleTree() {
    TreeNode *a = new TreeNode('A');
    TreeNode *b = new TreeNode('B');
    TreeNode *c = new TreeNode('C');
    TreeNode *d = new TreeNode('D');
    TreeNode *e = new TreeNode('E');
    TreeNode *f = new TreeNode('F');
    TreeNode *g = new TreeNode('G');

    a->firstChild = b;          // A 的第一个孩子是 B
    b->nextSibling = c;         // B 的兄弟是 C
    c->nextSibling = d;         // C 的兄弟是 D
    b->firstChild = e;          // B 的孩子是 E
    e->nextSibling = f;         // E 的兄弟是 F
    d->firstChild = g;          // D 的孩子是 G
    return a;
}

// ---------- 任务 2：树的三种遍历 ----------
void preOrderTree(TreeNode *p) {
    if (!p) return;
    std::printf("%c", p->data);
    for (TreeNode *c = p->firstChild; c; c = c->nextSibling)
        preOrderTree(c);
}

void postOrderTree(TreeNode *p) {
    if (!p) return;
    for (TreeNode *c = p->firstChild; c; c = c->nextSibling)
        postOrderTree(c);
    std::printf("%c", p->data);
}

void levelOrderTree(TreeNode *p) {
    if (!p) return;
    std::queue<TreeNode *> q;
    q.push(p);
    while (!q.empty()) {
        TreeNode *cur = q.front();
        q.pop();
        std::printf("%c", cur->data);
        for (TreeNode *c = cur->firstChild; c; c = c->nextSibling)
            q.push(c);
    }
}

// ---------- 任务 3：树 -> 二叉树 ----------
BinNode *treeToBinary(TreeNode *p) {
    if (!p) return nullptr;
    BinNode *q = new BinNode(p->data);
    q->left = treeToBinary(p->firstChild);    // 第一个孩子 -> 左子女
    q->right = treeToBinary(p->nextSibling);  // 下一个兄弟 -> 右子女
    return q;
}

// ---------- 任务 4：二叉树的遍历 ----------
void preOrderBin(BinNode *p) {
    if (!p) return;
    std::printf("%c", p->data);
    preOrderBin(p->left);
    preOrderBin(p->right);
}

void inOrderBin(BinNode *p) {
    if (!p) return;
    inOrderBin(p->left);
    std::printf("%c", p->data);
    inOrderBin(p->right);
}

// ---------- 任务 5：森林 -> 二叉树 ----------
BinNode *forestToBinary(TreeNode **roots, int n) {
    if (n == 0) return nullptr;
    BinNode *first = treeToBinary(roots[0]);
    BinNode *cur = first;
    for (int i = 1; i < n; ++i) {
        cur->right = treeToBinary(roots[i]);  // 后一棵树挂到前一棵的右链
        cur = cur->right;
    }
    return first;
}

// ---------- 任务 6：二叉树 -> 森林 ----------
// 打印以 root 为根的那棵树（忽略 root 的右指针，它是"下一棵树的根"）。
static void printOneTree(BinNode *root) {
    if (!root) return;
    std::printf("%c", root->data);
    for (BinNode *c = root->left; c; c = c->right)
        printOneTree(c);
}

void splitForest(BinNode *root) {
    int k = 0;
    for (BinNode *t = root; t; t = t->right) {
        std::printf("T%d: ", ++k);
        printOneTree(t);
        std::printf("\n");
    }
}

// ---------- 销毁 ----------
void destroyTree(TreeNode *p) {
    if (!p) return;
    TreeNode *c = p->firstChild;
    while (c) {
        TreeNode *next = c->nextSibling;
        destroyTree(c);
        c = next;
    }
    delete p;
}

void destroyBin(BinNode *p) {
    if (!p) return;
    destroyBin(p->left);
    destroyBin(p->right);
    delete p;
}

int main() {
    TreeNode *root = buildSampleTree();
    std::printf("=== 任务 1：树 A(B(E,F),C,D(G)) ===\n");
    std::printf("树的前序：");
    preOrderTree(root);
    std::printf("\n树的后序：");
    postOrderTree(root);
    std::printf("\n树的层次：");
    levelOrderTree(root);
    std::printf("\n\n");

    std::printf("=== 任务 2：树 -> 二叉树 ===\n");
    BinNode *bin = treeToBinary(root);
    std::printf("二叉树前序：");
    preOrderBin(bin);
    std::printf("\n二叉树中序：");
    inOrderBin(bin);
    std::printf("\n");
    std::printf("对应关系：树前序 = 二叉树前序（ABEFCDG）\n");
    std::printf("          树后序 = 二叉树中序（EFBCGDA）\n\n");

    std::printf("=== 任务 3：森林 -> 二叉树 ===\n");
    TreeNode *t1 = new TreeNode('A');
    TreeNode *b = new TreeNode('B');
    TreeNode *c = new TreeNode('C');
    t1->firstChild = b;
    b->nextSibling = c;

    TreeNode *t2 = new TreeNode('D');
    t2->firstChild = new TreeNode('E');

    TreeNode *t3 = new TreeNode('F');
    TreeNode *g = new TreeNode('G');
    TreeNode *h = new TreeNode('H');
    t3->firstChild = g;
    g->nextSibling = h;

    TreeNode *forest[3] = {t1, t2, t3};
    std::printf("森林先根：");
    for (int i = 0; i < 3; ++i) preOrderTree(forest[i]);
    std::printf("\n森林后根：");
    for (int i = 0; i < 3; ++i) postOrderTree(forest[i]);
    std::printf("\n");

    BinNode *fbin = forestToBinary(forest, 3);
    std::printf("二叉树前序：");
    preOrderBin(fbin);
    std::printf("\n二叉树中序：");
    inOrderBin(fbin);
    std::printf("\n");
    std::printf("对应关系：森林先根 = 二叉树前序（ABCDEFGH）\n");
    std::printf("          森林后根 = 二叉树中序（BCAEDGHF）\n\n");

    std::printf("=== 任务 4：二叉树 -> 森林 ===\n");
    splitForest(fbin);
    std::printf("\n");

    destroyTree(root);
    destroyTree(t1);
    destroyTree(t2);
    destroyTree(t3);
    destroyBin(bin);
    destroyBin(fbin);
    return 0;
}
