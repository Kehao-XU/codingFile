// Lesson3/src/linkedlist.cpp
// 单链表四个基本操作：尾插建表、原地反转、合并有序表、约瑟夫环。
// 对应学案 03 第五节。
// 构建：g++ -O2 -std=c++17 src/linkedlist.cpp -o output/linkedlist.exe
//
// 练习骨架：TODO 处自己补，参考答案在 solution/linkedlist.cpp。

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

// TODO: 尾插建表。建一个表头结点（data 不用），把 a[0..n-1] 按原顺序接在后面。
//       要求：用尾指针，全程 O(n)；不要每插一个都从头找尾。
//       自测：{1,2,3,4,5} 建成表后，从 head->link 开始打印应为 1 2 3 4 5。
Node *createByTail(const int *a, int n) {
    // TODO
    Node *head = new Node;
    Node *current = head;
    for (int i = 0; i < n; ++i) {
        Node *tail = newNode(a[i], nullptr);
        current->link = tail;
        current = tail;
    }
    return head;
}

void print(const char *tag, Node *head) {
    std::printf("%s", tag);
    for (Node *p = head->link; p; p = p->link)
        std::printf("%d ", p->data);
    std::printf("\n");
}

// TODO: 原地反转带头结点的单链表，只改 link 指针，不申请新结点、不改 data。
//       三个指针 prev / cur / next 足够，循环结束后把 head->link
//       接上新的首结点。 自测：1 2 3 4 5 反转后打印应为 5 4 3 2 1。
void reverse(Node *head) {
    // TODO
    if (head == nullptr || head->link == nullptr)
        return;
    if (head->link->link == nullptr)
        return;

    Node *prev, *cur, *next;
    prev = head->link;
    cur = prev->link;
    prev->link = nullptr;
    next = cur->link;

    while (next != nullptr) {
        cur->link = prev;
        prev = cur;
        cur = next;
        next = next->link;
    }
    cur->link = prev;
    head->link = cur;
}

void destroy(Node *head) {
    while (head) {
        Node *p = head->link;
        delete head;
        head = p;
    }
}

// TODO: 合并两个有序（非降）单链表，复用原结点，结果仍带头结点。
//       要点：工作指针在结果链尾走；某个表走完后，把另一个表的剩余整段直接挂上，
//             不要逐个结点搬。两个输入的表头结点不在结果里复用。
//       自测：{1,3,5} 与 {2,4,6} 合并后应为 1 2 3 4 5 6。
Node *mergeSorted(Node *a, Node *b) {
    // TODO
    if (a == nullptr || b == nullptr || a->link == nullptr ||
        b->link == nullptr)
        return nullptr;

    Node *head = new Node;
    Node *curA = a->link, *curB = b->link, *current = head;

    while (curA != nullptr && curB != nullptr) {
        if (curA->data <= curB->data) {
            current->link = curA;
            current = curA;
            curA = curA->link;
        } else {
            current->link = curB;
            current = curB;
            curB = curB->link;
        }
    }

    if (curA != nullptr)
        current->link = curA;
    if (curB != nullptr)
        current->link = curB;

    return head;
}

// ---------- 循环链表：约瑟夫 ----------
struct CircNode {
    int data;
    CircNode *link;
};

// TODO: 约瑟夫环，n 个人编号 1..n 围成单循环链表，从 1 号开始报数，报到 m
// 的出列。
//       输出格式："约瑟夫 n=%d m=%d 出列顺序："
//       后依次打印出列编号，用空格分隔，
//                 最后打印 "｜幸存者：%d"（全角竖线）。
//       要点：pre 始终指向 p 的前驱，摘结点时 pre->link = p->link。
//       返回最后剩下那个人的编号；调用者不负责释放，函数内要 delete 干净。
//       自测：josephus(8, 3) 幸存者应为 7。
int josephus(int n, int m) {
    // TODO
    return 0;
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
    delete A; // A、B 的表头结点没被复用，单独释放
    delete B;
    destroy(C);

    // 4. 约瑟夫
    josephus(8, 3);
    return 0;
}
