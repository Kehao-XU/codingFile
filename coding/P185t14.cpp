#include<stdio.h>
#include<malloc.h>

struct Node{
    int val;
    Node *next;
};

void printList(Node *head){
    Node *p=head;
    if(!p){
        printf("NULL");
        return;
    }
    while(p){
        printf("%d",p->val);
        p=p->next;
    }
    printf("\n");
}

void freeList(Node* head){  // 只释放头节点及其后面的节点
    Node* temp=head;
    while(head){
        head=head->next;
        free(temp);
        temp=head;
    }
}

void mergeList(Node *head1,Node *head2){
    Node *p1=head1->next,*p2=head2->next;
    Node *tail = head1;  // 使用tail指向新链表的尾部
    
    // 清空head1的原有内容
    head1->next = NULL;
    
    // 合并两个链表
    while(p1 && p2){
        if(p1->val < p2->val){
            tail->next = p1;
            tail = p1;
            p1 = p1->next;
        }
        else if(p1->val > p2->val){
            tail->next = p2;
            tail = p2;
            p2 = p2->next;
        }
        else{
            // 值相等时，只保留一个节点
            tail->next = p1;
            tail = p1;
            p1 = p1->next;
            
            // 释放p2指向的节点
            Node *temp = p2;
            p2 = p2->next;
            free(temp);
        }
    }
    
    // 连接剩余的节点
    if(p1){
        tail->next = p1;
    }else if(p2){
        tail->next = p2;
    }
}

void setDifference(Node *head1,Node *head2){
    Node *p1=head1->next,*p2=head2->next,*tail=head1;
    head1->next=NULL;

    while(p1&&p2){
       if(p1->val < p2->val){
            tail->next = p1;
            tail = p1;
            p1 = p1->next;
        }
        else if(p1->val > p2->val){
            Node *temp=p2;
            p2 = p2->next;
            free(temp);
        }
        else{
            Node *temp1=p1,*temp2=p2;
            p1 = p1->next;
            p2 = p2->next;
            free(temp1),free(temp2);
        }
    }

    if(p2)freeList(p2);
}

void setIntersection(Node *head1,Node *head2){
    Node *p1=head1->next,*p2=head2->next,*tail=head1;
    head1->next=NULL;

    while(p1&&p2){
       if(p1->val < p2->val){
            Node *temp=p1;
            p1 = p1->next;
            free(temp);
        }
        else if(p1->val > p2->val){
            Node *temp=p2;
            p2 = p2->next;
            free(temp);
        }
        else{
            Node *temp2=p2;
            tail->next=p1;
            tail=p1;
            p1 = p1->next;
            p2 = p2->next;
            free(temp2);
        }
    }

    tail->next=NULL;

    if(p2)freeList(p2);
}

int main(){
    Node *head1=NULL,*head2=NULL;
    head1=(Node*)malloc(sizeof(Node));
    if(head1==NULL){
            printf("Memory allocation failed!\n");
            return 0;
    }

    head2=(Node*)malloc(sizeof(Node));
    if(head2==NULL){
            printf("Memory allocation failed!\n");
            free(head1);
            return 0;
    }

    // 创建链表1: 1 -> 2 -> 3 -> 6 -> 8
    Node *a1=(Node*)malloc(sizeof(Node)),*a2=(Node*)malloc(sizeof(Node)),
    *a3=(Node*)malloc(sizeof(Node)),*a4=(Node*)malloc(sizeof(Node)),*a5=(Node*)malloc(sizeof(Node));
    
    // 创建链表2: 4 -> 5 -> 7
    Node *b1=(Node*)malloc(sizeof(Node)),*b2=(Node*)malloc(sizeof(Node)),*b3=(Node*)malloc(sizeof(Node));

    head1->next=a1;
    a1->val=1,a2->val=2,a3->val=4,a4->val=6,a5->val=8;
    a1->next=a2,a2->next=a3,a3->next=a4,a4->next=a5,a5->next=NULL;
    
    head2->next=b1;
    b1->val=4,b2->val=5,b3->val=7;
    b1->next=b2,b2->next=b3,b3->next=NULL;

    setIntersection(head1,head2);
    printList(head1->next);
    // 只释放第一个链表的内存，因为第二个链表的节点已经被合并到第一个链表中
    freeList(head1);
    // 将head2置空，避免后续访问
    head2->next = NULL;
    free(head2);

    return 0;
}