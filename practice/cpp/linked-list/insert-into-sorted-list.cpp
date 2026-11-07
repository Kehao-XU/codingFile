/*定义一个链表并赋初值，使用插入排序对链表进行排序
每轮循环中，插入排序只从输入数据中移除一个待排序的元素，找到它在序列中适当的位置，并将其插入。
*/
#include<stdio.h>
#include<malloc.h>
#include"SimpleNode.h"
void insertAscend(Node* head){
    Node *ascendMax=head->next;
    while(ascendMax->next&&ascendMax->next->value>ascendMax->value)ascendMax=ascendMax->next;

    Node *current=ascendMax->next;
    if(!current){
        printf("Already ordered ascendLink.\n");
        return;
    }

    ascendMax->next=NULL;

    while(current){
        Node *p=head->next;
        Node *temp=current;
        current=current->next;
        temp->next=NULL;

        if(temp->value<p->value){
            head->next=temp;
            temp->next=p;
        }

        else{
            while(p->next&&temp->value>p->next->value){
                p=p->next;
            }

            if(!p->next)p->next=temp;
            else{
                temp->next=p->next;
                p->next=temp;
            }
        }
    }
}

int main(){
    int n;
    printf("Enter the length of the link.\n");
    scanf("%d",&n);

    Node *head=createList(n);
    printList(head->next);
    insertAscend(head);
    printf("After insertAscend:\n");
    printList(head->next);

    freeList(head);
    return 0;
}