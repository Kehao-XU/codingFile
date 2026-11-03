// Lesson3/src/linkedlist.cpp
// 单链表四个基本操作：尾插建表、原地反转、合并有序表、约瑟夫环。
// 对应学案 03 第五节。
// 构建：g++ -O2 -std=c++17 src/linkedlist.cpp -o output/linkedlist.exe

#include <cstdio>

struct Node {
    int data;
    Node *link;
};

// ---------- 带头结点的单链表 ----------
Node *newNode(int v, Node *next = nullptr) {
    Node *p = new Node;
    p->data = v;
    p->link = next;
    return p;
}

// 建表：表头结点 + 尾指针，保持输入顺序
Node *createByTail(const int *a, int n) {
    Node *head = newNode(0);          // 表头结点，data 不用
    Node *last = head;
    for (int i = 0; i < n; ++i) {
        last->link = newNode(a[i]);
        last = last->link;            // 尾指针前移，避免每次从头找尾
    }
    return head;
}

void print(const char *tag, Node *head) {
    std::printf("%s", tag);
    for (Node *p = head->link; p; p = p->link)
        std::printf("%d ", p->data);
    std::printf("\n");
}

// 原地反转：只改指针，不申请新结点
void reverse(Node *head) {
    Node *prev = nullptr;
    Node *cur = head->link;
    while (cur) {
        Node *next = cur->link;       // 1. 先记住后继
        cur->link = prev;             // 2. 掉头
        prev = cur;                   // 3. prev 前进
        cur = next;                   // 4. cur 前进
    }
    head->link = prev;
}

void destroy(Node *head) {
    while (head) {
        Node *p = head->link;
        delete head;
        head = p;
    }
}

// 合并两个有序单链表，复用原结点，结果仍带头结点
Node *mergeSorted(Node *a, Node *b) {
    Node *head = newNode(0);
    Node *tail = head;
    Node *pa = a->link, *pb = b->link;
    while (pa && pb) {
        if (pa->data <= pb->data) {
            tail->link = pa;
            pa = pa->link;
        } else {
            tail->link = pb;
            pb = pb->link;
        }
        tail = tail->link;
    }
    tail->link = pa ? pa : pb;        // 把剩下的整段挂上，不用逐个搬
    return head;
}

// ---------- 循环链表：约瑟夫 ----------
struct CircNode {
    int data;
    CircNode *link;
};

// 返回出列顺序，出列的人从链上删掉，剩下最后一个返回其编号
int josephus(int n, int m) {
    CircNode *first = new CircNode{1, nullptr};
    CircNode *last = first;
    for (int i = 2; i <= n; ++i) {
        last->link = new CircNode{i, nullptr};
        last = last->link;
    }
    last->link = first;               // 首尾相接，成环

    CircNode *pre = last;             // pre 始终是 p 的前驱
    CircNode *p = first;
    std::printf("约瑟夫 n=%d m=%d 出列顺序：", n, m);
    for (int k = 0; k < n - 1; ++k) { // 淘汰 n-1 人
        for (int j = 1; j < m; ++j) { // 报数 1..m-1，停在第 m 个人
            pre = p;
            p = p->link;
        }
        std::printf("%d ", p->data);
        pre->link = p->link;          // 从环上摘掉
        delete p;
        p = pre->link;
    }
    int survivor = p->data;
    std::printf("｜幸存者：%d\n", survivor);
    delete p;
    return survivor;
}

int main() {
    // 1. 尾插建表
    int a[] = {1, 2, 3, 4, 5};
    Node *L = createByTail(a, 5);
    print("尾插建表：      ", L);

    // 2. 原地反转
    reverse(L);
    print("反转后：        ", L);
    destroy(L);

    // 3. 合并两个有序表
    int x[] = {1, 3, 5};
    int y[] = {2, 4, 6};
    Node *A = createByTail(x, 3);
    Node *B = createByTail(y, 3);
    Node *C = mergeSorted(A, B);
    print("合并 {1,3,5}+{2,4,6}：", C);
    delete A;                         // A、B 的表头结点没被复用，单独释放
    delete B;
    destroy(C);

    // 4. 约瑟夫
    josephus(8, 3);
    return 0;
}
