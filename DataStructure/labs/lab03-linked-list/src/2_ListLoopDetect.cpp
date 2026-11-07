/**
 * 给定一个链表的头节点  head ，返回链表开始入环的第一个节点。 如果链表无环，则返回 null。
如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。
为了表示给定链表中的环，评测系统内部使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。
如果 pos 是 -1，则在该链表中没有环。
注意：pos 不作为参数进行传递，仅仅是为了标识链表的实际情况。
 */

#include <iostream>
#include <vector>

struct Node{
    int val;
    Node* next;
};

const Node* solution(const Node* head) {
    const Node* slow = head;
    const Node* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) break;
    }
    if (fast == nullptr || fast->next == nullptr) {
        return nullptr;
    }

    const Node* p = head;
    while (p != slow) {
        p = p->next;
        slow = slow->next;
    }
    return p;
}

// 把尾节点接到索引 pos 的节点上，pos = -1 表示不接环（模拟评测系统）
Node* makeCycle(Node* head, int pos) {
    if (pos < 0 || head == nullptr) return head;
    Node* tail = head;
    while (tail->next != nullptr) tail = tail->next;
    Node* entry = head;
    for (int i = 0; i < pos; i++) entry = entry->next;
    tail->next = entry;
    return head;
}

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

int main() {
    std::vector<std::pair<std::vector<int>, int>> cases = {
        {{3, 2, 0, -4}, 1},
        {{1, 2}, 0},
        {{1}, -1},
        {{}, -1},
    };

    for (const auto& [values, pos] : cases) {
        Node* head = makeCycle(buildList(values), pos);
        const Node* entry = solution(head);
        std::cout << "pos = " << pos << " -> 返回 ";
        if (entry != nullptr) {
            std::cout << "节点 val = " << entry->val << std::endl;
        } else {
            std::cout << "nullptr" << std::endl;
        }
    }
    return 0;
}