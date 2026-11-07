/*定义一个链表并赋初值，遍历链表，然后两两交换其中相邻的节点，最后再遍历链表。
你必须在不修改节点内部的值的情况下完成本题（即，只能进行节点交换，而非数值交换）。
*/
#include<stdio.h>
#include<malloc.h>
#include"SimpleNode.h"

void switchNodeLink(Node* head){
    Node *current=head->next,*p=head;
    int count=0;

    while(current){
        current=current->next;
        count++;

        if(count==2){
            count=0;
            p->next->next->next=p->next;
            p->next=p->next->next;
            p->next->next->next=current;
            p=p->next->next;
        }
    }
}

int main(){
    int n;
    printf("Enter the length of the link.\n");
    scanf("%d",&n);

    Node *head=createList(n);
    printList(head->next);
    switchNodeLink(head);
    printf("After switchNode:\n");
    printList(head->next);

    freeList(head);
    return 0;
}