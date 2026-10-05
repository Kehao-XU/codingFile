/**
 * 给你单链表的头节点head，请你反转链表，并返回反转后的链表
 */

#include <iostream>
#include <vector>

struct Node{
    int val;
    Node* next;
};

/**
 * 反转单链表（迭代法）
 * @param head 原链表的头节点指针
 * @return 反转后链表的头节点指针
 *
 * 思路：用 prev / current / next 三个指针在链表上滚动。
 * 每走一步，先把 current->next 存进 next（防止断链后找不到后继），
 * 再让 current->next 指向 prev（完成一次“掉头”），
 * 最后 prev、current 各向前移动一格。
 * 循环结束时 current 为 nullptr，prev 指向原链表的尾节点，即新的头节点。
 * 时间复杂度 O(n)，空间复杂度 O(1)。
 */
Node* solution(Node* head) {
    Node* prev = nullptr;
    Node* current = head;

    while (current != nullptr) {
        Node* next = current->next;  // 1. 保存后继节点
        current->next = prev;        // 2. 反转当前节点的指针方向
        prev = current;              // 3. prev 前移
        current = next;              // 4. current 前移
    }

    return prev;                     // prev 即反转后的新头节点
}

/* ---------------- 以下为测试用的辅助函数 ---------------- */

// 用数组构建链表，返回头节点
Node* buildList(const std::vector<int>& values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int value : values) {
        Node* node = new Node{value, nullptr};
        if (head == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// 按 “1 -> 2 -> 3 -> null” 的形式打印链表
void printList(const Node* head) {
    const Node* current = head;
    while (current != nullptr) {
        std::cout << current->val << " -> ";
        current = current->next;
    }
    std::cout << "null" << std::endl;
}

// 释放链表全部节点，避免内存泄漏
void freeList(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

// 对一组数据完成“构建 -> 反转 -> 打印 -> 释放”
void testCase(const std::vector<int>& values) {
    Node* head = buildList(values);
    std::cout << "反转前: ";
    printList(head);

    head = solution(head);

    std::cout << "反转后: ";
    printList(head);
    std::cout << std::endl;

    freeList(head);
}

int main() {
    testCase({1, 2, 3, 4, 5});  // 普通情况
    testCase({1});              // 只有一个节点
    testCase({});               // 空链表
    return 0;
}
