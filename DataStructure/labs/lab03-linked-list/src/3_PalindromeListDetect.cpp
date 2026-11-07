/**
 * 给你一个单链表的头节点head，请你判断该链表是否为回文链表。
 */

#include <iostream>
#include <vector>

struct Node{
    int val;
    Node* next;
};

bool solution(Node* head) {
    if (head == nullptr) return true;
    if (head->next == nullptr) return true;
    
    Node* slow = head;
    Node* fast = head;
    Node* prevSlow = nullptr;

    while (fast != nullptr && fast->next != nullptr) {
        fast = fast->next->next;
        prevSlow = slow;
        slow = slow->next;
    }

    //从slow开始反转后面的链表
    if (fast != nullptr) {
        slow = slow->next;
        prevSlow = prevSlow->next;
    }

    Node* prev = nullptr;
    Node* temp = nullptr;
    while (slow != nullptr) {
        temp = slow->next;
        slow->next = prev;
        prev = slow;
        slow = temp;
    }
    slow = prev;

    //反转结束之后prev指向原链表的末尾，也就是现在后半段链表的表头，两端逐节点比较
    Node *p1 = head, *p2 = slow;
    bool ans = true;
    while (p2 != nullptr) {
        if (p1->val != p2->val) {
            ans = false;
            break;
        }
        if(p2->next == nullptr) {
            break; 
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    prev = nullptr;
    temp = nullptr;
    while (slow != nullptr) {
        temp = slow->next;
        slow->next = prev;
        prev = slow;
        slow = temp;
    }
    prevSlow->next = prev;

    return ans;
}

/* ---------------- 以下为测试部分，无需改动 ---------------- */

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

// Floyd 判环：检查链表是否被破坏成了环
bool hasCycle(const Node* head) {
    const Node* slow = head;
    const Node* fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true;
        }
    }
    return false;
}

// 按 “1 -> 2 -> 2 -> 1 -> null” 的形式打印链表
// 最多打印 30 个节点：万一链表被破坏成环，这里也不会死循环刷屏
void printList(const Node* head) {
    const Node* current = head;
    int count = 0;
    while (current != nullptr && count < 30) {
        std::cout << current->val << " -> ";
        current = current->next;
        count++;
    }
    if (current == nullptr) {
        std::cout << "null";
    } else {
        std::cout << "...（超过 30 个节点，可能成环）";
    }
}

// 释放链表全部节点；若链表已被破坏成环则跳过释放，避免陷入死循环
void freeList(Node* head) {
    if (hasCycle(head)) {
        return;
    }
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // 测试数据 + 期望结果（注意：空链表和单节点都算回文）
    const std::vector<std::pair<std::vector<int>, bool>> cases = {
        {{1, 2, 2, 1},    true},   // 偶数长度，是回文
        {{1, 2, 3, 2, 1}, true},   // 奇数长度，是回文
        {{1, 2, 3, 1, 2}, false},  // 奇数长度，不是回文
        {{1, 2},          false},  // 两个不同的值
        {{1, 1},          true},   // 两个相同的值
        {{1},             true},   // 只有一个节点
        {{},              true},   // 空链表
    };

    int passed = 0;
    for (size_t i = 0; i < cases.size(); i++) {
        const std::vector<int>& values = cases[i].first;
        const bool expected = cases[i].second;

        Node* head = buildList(values);

        std::cout << "用例 " << i + 1 << ": ";
        printList(head);
        std::cout << std::endl;

        const bool actual = solution(head);

        std::cout << "  期望: " << (expected ? "true" : "false")
                  << "  实际: " << (actual ? "true" : "false")
                  << "  " << (actual == expected ? "[PASS]" : "[FAIL]") << std::endl;

        // 检查 solution 是否破坏了原链表（进阶写法必须还原链表）
        std::cout << "  调用后链表: ";
        printList(head);
        std::cout << std::endl;
        if (hasCycle(head)) {
            std::cout << "  警告: 链表已被破坏成环，说明 solution 里还原链表的部分有 bug" << std::endl;
        }
        std::cout << std::endl;

        if (actual == expected) {
            passed++;
        }
        freeList(head);
    }

    std::cout << "通过 " << passed << " / " << cases.size() << " 个用例" << std::endl;
    return 0;
}