// Lesson8/src/threaded.cpp
// 补充练习：中序线索二叉树（建线索、找前驱/后继、无栈遍历）。
// 对应学案 08 第二节。
// 构建：g++ -O2 -std=c++17 src/threaded.cpp -o output/threaded.exe
//
// 练习骨架：TODO 处自己补，参考答案在 solution/threaded.cpp。

#include <cstdio>

struct ThreadNode {
    char data;
    ThreadNode *left, *right;
    int ltag, rtag;  // 0 = 指向真子女，1 = 指向前驱/后继线索
    ThreadNode(char d)
        : data(d), left(nullptr), right(nullptr), ltag(0), rtag(0) {}
};

// 由前序 + 中序建树（已给好，专注线索部分）
ThreadNode *buildFromPreIn(const char *vlr, const char *lvr, int n) {
    if (n == 0) return nullptr;
    char root = vlr[0];
    int k = 0;
    while (lvr[k] != root) ++k;
    ThreadNode *p = new ThreadNode(root);
    p->left = buildFromPreIn(vlr + 1, lvr, k);
    p->right = buildFromPreIn(vlr + k + 1, lvr + k + 1, n - k - 1);
    return p;
}

// ---------- 任务 1：中序线索化 ----------
// 中序遍历一遍，pre 始终是"上一个访问的结点"。
//   若 p 的左指针为空：p->left = pre，ltag = 1；
//   若 pre 的右指针为空：pre->right = p，rtag = 1；
//   然后 pre = p，继续遍历。
// TODO 1
void inThread(ThreadNode *p, ThreadNode *&pre) {
    // TODO
    if (!p) return;
    inThread(p->left, pre);

    if (!p->left) {
        p->left = pre;
        p->ltag = 1;
    }
    if (pre && !pre->right) {
        pre->right = p;
        pre->rtag = 1;
    }
    if (!p->right) p->rtag = 1;
    pre = p;

    inThread(p->right, pre);
}

// ---------- 任务 2：在线索上找第一个结点 / 后继 / 前驱 ----------
// 中序第一个结点：从根沿左链一直走，直到 ltag == 1。
// TODO 2
ThreadNode *first(ThreadNode *p) {
    // TODO
    while (p && !p->ltag && p->left) p = p->left;
    return p;
}

// 中序最后一个结点：从根沿右链一直走，直到 rtag == 1。
// TODO 3
ThreadNode *last(ThreadNode *p) {
    // TODO
    while (p && !p->rtag && p->right) p = p->right;
    return p;
}

// 后继：rtag == 1 直接取 right；否则是右子树的中序第一个结点。
// TODO 4
ThreadNode *next(ThreadNode *p) {
    // TODO
    return nullptr;
}

// 前驱：ltag == 1 直接取 left；否则是左子树的中序最后一个结点。
// TODO 5
ThreadNode *prev(ThreadNode *p) {
    // TODO
    return nullptr;
}

// ---------- 任务 3：无栈中序遍历 ----------
// TODO 6
void inOrderThread(ThreadNode *root) {
    // TODO
    for (ThreadNode *p = first(root); p; p = next(p)) ;
}

// ---------- 任务 4：统计线索个数 ----------
// 线索总数应等于空指针域个数 n+1。
// 注意：只递归真子女，不能顺着 ltag/rtag 走进线索，否则会绕圈。
// TODO 7
int countThreads(ThreadNode *p) {
    // TODO
    if (!p) return 0;
    int count = 0;
    if (p->left && !p->ltag) count += countThreads(p->left);
    count += p->ltag + p->rtag;
    if (p->right && !p->rtag) count +=  countThreads(p->right);
    return count;
}

// ---------- 辅助：递归遍历（未线索化时用于核对） ----------
void preOrderRec(ThreadNode *p) {
    if (!p) return;
    std::printf("%c", p->data);
    if (p->ltag == 0) preOrderRec(p->left);
    if (p->rtag == 0) preOrderRec(p->right);
}

void inOrderRec(ThreadNode *p) {
    if (!p) return;
    if (p->ltag == 0) inOrderRec(p->left);
    std::printf("%c", p->data);
    if (p->rtag == 0) inOrderRec(p->right);
}

void printNode(ThreadNode *p) {
    if (p) std::printf("%c", p->data);
    else std::printf("-");
}

void destroy(ThreadNode *p) {
    if (!p) return;
    if (p->ltag == 0) destroy(p->left);
    if (p->rtag == 0) destroy(p->right);
    delete p;
}

// 建一条只有左子女的链：a -> b -> c
ThreadNode *buildLeftChain() {
    ThreadNode *a = new ThreadNode('A');
    ThreadNode *b = new ThreadNode('B');
    ThreadNode *c = new ThreadNode('C');
    a->left = b;
    b->left = c;
    return a;
}

// 建一条只有右子女的链：a -> b -> c
ThreadNode *buildRightChain() {
    ThreadNode *a = new ThreadNode('A');
    ThreadNode *b = new ThreadNode('B');
    ThreadNode *c = new ThreadNode('C');
    a->right = b;
    b->right = c;
    return a;
}

int main() {
    // ---------- 任务 1：建树并线索化 ----------
    std::printf("=== 任务 1：建树 ===\n");
    const char *vlr = "ABHFDECKG";
    const char *lvr = "HBDFAEKCG";
    ThreadNode *root = buildFromPreIn(vlr, lvr, 9);
    std::printf("前序：");
    preOrderRec(root);
    std::printf("\n中序：");
    inOrderRec(root);
    std::printf("\n\n");

    std::printf("=== 任务 2：中序线索化 ===\n");
    ThreadNode *pre = nullptr;
    inThread(root, pre);
    std::printf("线索总数：%d（应为 n+1 = 10）\n", countThreads(root));
    std::printf("中序第一个结点：");
    printNode(first(root));
    std::printf("，中序最后一个结点：");
    printNode(last(root));
    std::printf("\n\n");

    // ---------- 任务 2：无栈遍历 ----------
    std::printf("=== 任务 3：无栈中序遍历 ===\n");
    inOrderThread(root);
    std::printf("\n\n");

    // ---------- 任务 3：前驱后继表 ----------
    std::printf("=== 任务 4：前驱 / 后继表 ===\n");
    std::printf("%-6s %-6s %-6s\n", "结点", "前驱", "后继");
    for (ThreadNode *p = first(root); p; p = next(p)) {
        std::printf("%-6c ", p->data);
        printNode(prev(p));
        std::printf("      ");
        printNode(next(p));
        std::printf("\n");
    }
    std::printf("\n");

    // ---------- 任务 4：边界情况 ----------
    std::printf("=== 任务 5：边界情况 ===\n");
    std::printf("空树：");
    inOrderThread(nullptr);
    std::printf("\n");

    ThreadNode *single = new ThreadNode('X');
    pre = nullptr;
    inThread(single, pre);
    std::printf("单结点：");
    inOrderThread(single);
    std::printf("（线索数 %d，前驱 ", countThreads(single));
    printNode(prev(first(single)));
    std::printf("，后继 ");
    printNode(next(first(single)));
    std::printf("）\n");

    ThreadNode *leftChain = buildLeftChain();
    pre = nullptr;
    inThread(leftChain, pre);
    std::printf("左链 ABC 的中序：");
    inOrderThread(leftChain);
    std::printf("\n");

    ThreadNode *rightChain = buildRightChain();
    pre = nullptr;
    inThread(rightChain, pre);
    std::printf("右链 ABC 的中序：");
    inOrderThread(rightChain);
    std::printf("\n");

    destroy(root);
    destroy(single);
    destroy(leftChain);
    destroy(rightChain);
    return 0;
}
