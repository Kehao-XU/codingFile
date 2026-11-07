#include<stdio.h>
#include<malloc.h>
struct Node{
    int value;
    Node *next;
}

Node* createNode(int n){
    Node *head=(Node*)malloc(sizeof(Node));
    if(!head){
        printf("Memory allocation failed!\n");
        return NULL;
    }

    head->value=n;
    head->next=NULL;

    return head;
}

void appendNode(Node* head,int n){
    Node *tail=head,*p=(Node*)malloc(sizeof(Node));
    if(!p){
        printf("Memory allocation failed!\n");
        return;
    }

    p->value=n;
    p->next=NULL;

    while(!tail->next)tail=tail->next;
    tail->next=p;
}

