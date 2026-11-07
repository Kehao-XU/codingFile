// Lesson7/src/tree_forest.cpp
// 补充练习：树、森林与二叉树的转换（左孩子-右兄弟）。
// 对应学案 07 第八节。
// 构建：g++ -O2 -std=c++17 src/tree_forest.cpp -o output/tree_forest.exe
//
// 练习骨架：TODO 处自己补，参考答案在 solution/tree_forest.cpp。

#include <cstdio>
#include <queue>

// ================= 两种结点 =================
// 树/森林：firstChild 指向第一个孩子，nextSibling 指向下一个兄弟。
struct TreeNode {
    char data;
    TreeNode *firstChild, *nextSibling;
    TreeNode(char d) : data(d), firstChild(nullptr), nextSibling(nullptr) {}
};

// 二叉树：left/right 是真正的左右子女。
struct BinNode {
    char data;
    BinNode *left, *right;
    BinNode(char d) : data(d), left(nullptr), right(nullptr) {}
};

// ---------- 任务 1：建树 A(B(E,F),C,D(G)) ----------
/*
        A
      / | \
     B  C  D
    / \    |
   E   F   G
*/
// 提示：孩子的兄弟关系用 nextSibling 串，例如 B->nextSibling = C。
// TODO 1：把 7 个结点接起来，返回根 A。
TreeNode *buildSampleTree() {
    // TODO
    TreeNode *A = new TreeNode('A');
    TreeNode *B = new TreeNode('B');
    A->firstChild = B;
    TreeNode *C = new TreeNode('C');

    return nullptr;
}

// ---------- 任务 2：树的三种遍历 ----------
// 前序（先根）：根 -> 各子树依次前序
// TODO 2：沿 firstChild，再沿 nextSibling 递归。
void preOrderTree(TreeNode *p) {
    // TODO
}

// 后序（后根）：各子树依次后序 -> 根
// TODO 3
void postOrderTree(TreeNode *p) {
    // TODO
}

// 层次：用 queue，出队访问，所有子女依次入队
// TODO 4
void levelOrderTree(TreeNode *p) {
    // TODO
}

// ---------- 任务 3：树 -> 二叉树 ----------
// 口诀：第一个孩子变左子女，下一个兄弟变右子女。
// TODO 5：返回新建的二叉树根（不要复用 TreeNode 指针，方便体会两种结构）。
BinNode *treeToBinary(TreeNode *p) {
    // TODO
    BinNode *bp = new BinNode(p->data);
    if (p->firstChild) bp->left = treeToBinary(p->firstChild);
    if (p->nextSibling) bp->right = treeToBinary(p->nextSibling);
    return bp;
}

// ---------- 任务 4：二叉树的遍历 ----------
// TODO 6
void preOrderBin(BinNode *p) {
    // TODO
}

// TODO 7
void inOrderBin(BinNode *p) {
    // TODO
}

// ---------- 任务 5：森林 -> 二叉树 ----------
// 各棵树先各自转换，再把第 2、3… 棵树的根作为前一根的右子女串起来。
// TODO 8
BinNode *forestToBinary(TreeNode **roots, int n) {
    // TODO
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
// 根的右链上每个结点就是一棵树的根。
// TODO 9：按右链拆开，打印每棵树的先根遍历，格式 "T1: A B C\n"。
void splitForest(BinNode *root) {
    // TODO
    if (!root) return;
    
    std::printf("%c ", root->data);
    preOrderBin(root->left);
    std::printf('\n');
    splitForest(root->right);
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
    // 任务 1~2：一棵树
    TreeNode *root = buildSampleTree();
    std::printf("=== 任务 1：树 A(B(E,F),C,D(G)) ===\n");
    std::printf("树的前序：");
    preOrderTree(root);
    std::printf("\n树的后序：");
    postOrderTree(root);
    std::printf("\n树的层次：");
    levelOrderTree(root);
    std::printf("\n\n");

    // 任务 3~4：树转二叉树，验证遍历对应关系
    std::printf("=== 任务 2：树 -> 二叉树 ===\n");
    BinNode *bin = treeToBinary(root);
    std::printf("二叉树前序：");
    preOrderBin(bin);
    std::printf("\n二叉树中序：");
    inOrderBin(bin);
    std::printf("\n");
    std::printf("对应关系：树前序 = 二叉树前序（ABEFCDG）\n");
    std::printf("          树后序 = 二叉树中序（EFBCGDA）\n\n");

    // 任务 5：森林转二叉树
    std::printf("=== 任务 3：森林 -> 二叉树 ===\n");
    // 森林 = { T1: A(B,C), T2: D(E), T3: F(G,H) }
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

    // 任务 6：反向拆森林
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
