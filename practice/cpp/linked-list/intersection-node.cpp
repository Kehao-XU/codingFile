#include<stdio.h>
#include<malloc.h>
struct Node{
    int data;
    Node *next;
};

Node* getIntersectionNode(Node* headA, Node* headB) {
    if (headA == NULL || headB == NULL) {
        return NULL;
    }
    
    Node *currentA = headA;
    Node *currentB = headB;
    
    while (currentA != currentB) {

        currentA = (currentA == NULL) ? headB : currentA->next;
        currentB = (currentB == NULL) ? headA : currentB->next;
    }
    

    return currentA;
}

int main() {
    // 公共部分
    struct Node* c1 = (Node*)malloc(sizeof(*c1));
    struct Node* c2 = (Node*)malloc(sizeof(*c2));
    c1->data = 8;  c1->next = c2;
    c2->data = 10; c2->next = NULL;
    // 链表 A：1 -> 2 -> 8 -> 10
    struct Node* a1 = (Node*)malloc(sizeof(*a1));
    struct Node* a2 = (Node*)malloc(sizeof(*a2));
    a1->data = 1; a1->next = a2;
    a2->data = 2; a2->next = c1;
    // 链表 B：3 -> 8 -> 10
    struct Node* b1 = (Node*)malloc(sizeof(*b1));
    b1->data = 3; b1->next = c1;

    struct Node* inter = getIntersectionNode(a1, b1);
    if (inter)
        printf("相交起始节点值：%d\n", inter->data);
    else
        printf("不相交\n");

    free(a1);
    free(a2);
    free(b1);
    free(c1);
    free(c2);
    
    return 0;
}