// Lesson8/src/threaded.cpp
// 补充练习：中序线索二叉树（建线索、找前驱/后继、无栈遍历）。
// 对应学案 08 第二节。
// 构建：g++ -O2 -std=c++17 src/threaded.cpp -o output/threaded.exe

#include <cstdio>

struct ThreadNode {
    char data;
    ThreadNode *left, *right;
    int ltag, rtag;  // 0 = 指向真子女，1 = 指向前驱/后继线索
    ThreadNode(char d)
        : data(d), left(nullptr), right(nullptr), ltag(0), rtag(0) {}
};

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
void inThread(ThreadNode *p, ThreadNode *&pre) {
    if (!p) return;
    inThread(p->left, pre);          // 左（此时 left 还是真子女）

    if (!p->left) {                  // 左空：指向前驱
        p->left = pre;
        p->ltag = 1;
    }
    if (pre && !pre->right) {        // 前驱右空：指向后继（当前 p）
        pre->right = p;
        pre->rtag = 1;
    }
    if (!p->right) {                 // 最后一个结点的右线索为空，但仍是线索
        p->rtag = 1;
    }
    pre = p;

    inThread(p->right, pre);         // 右（此时 right 还是真子女）
}

// ---------- 任务 2：第一个 / 最后一个 / 后继 / 前驱 ----------
ThreadNode *first(ThreadNode *p) {
    if (!p) return nullptr;
    while (p->ltag == 0 && p->left) p = p->left;
    return p;
}

ThreadNode *last(ThreadNode *p) {
    if (!p) return nullptr;
    while (p->rtag == 0 && p->right) p = p->right;
    return p;
}

ThreadNode *next(ThreadNode *p) {
    if (!p) return nullptr;
    if (p->rtag == 1) return p->right;   // 右指针就是后继线索
    return first(p->right);              // 否则是右子树的最左下结点
}

ThreadNode *prev(ThreadNode *p) {
    if (!p) return nullptr;
    if (p->ltag == 1) return p->left;    // 左指针就是前驱线索
    return last(p->left);                // 否则是左子树的最右下结点
}

// ---------- 任务 3：无栈中序遍历 ----------
void inOrderThread(ThreadNode *root) {
    for (ThreadNode *p = first(root); p; p = next(p))
        std::printf("%c", p->data);
}

// ---------- 任务 4：统计线索个数 ----------
int countThreads(ThreadNode *p) {
    if (!p) return 0;
    int self = p->ltag + p->rtag;
    if (p->ltag == 0) self += countThreads(p->left);
    if (p->rtag == 0) self += countThreads(p->right);
    return self;
}

// ---------- 辅助 ----------
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

ThreadNode *buildLeftChain() {
    ThreadNode *a = new ThreadNode('A');
    ThreadNode *b = new ThreadNode('B');
    ThreadNode *c = new ThreadNode('C');
    a->left = b;
    b->left = c;
    return a;
}

ThreadNode *buildRightChain() {
    ThreadNode *a = new ThreadNode('A');
    ThreadNode *b = new ThreadNode('B');
    ThreadNode *c = new ThreadNode('C');
    a->right = b;
    b->right = c;
    return a;
}

int main() {
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

    std::printf("=== 任务 3：无栈中序遍历 ===\n");
    inOrderThread(root);
    std::printf("\n\n");

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
