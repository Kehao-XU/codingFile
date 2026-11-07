#include<stdio.h>
#include<malloc.h>
struct Node{
    int value;
    Node* next;
};

void freeList(Node* head){
    Node *temp=head;
    while(head){
        head=head->next;
        free(temp);
        temp=head;
    }
}

void printList(Node* head){
    Node *temp=head;
    while(head->next){
        head=head->next;
        printf("%d->",temp->value);
        temp=head;
    }
    printf("%d\n",temp->value);
}

Node* createList(int n){
    Node *head=NULL,*tail=NULL,*p=NULL;
    head=(Node*)malloc(sizeof(Node));
    if(!head){
        printf("Memory allocation failed!\n");
        return NULL;
    }
    head->value=-1;
    head->next=NULL;

    for(int i=0;i<n;i++){
        p=(Node*)malloc(sizeof(Node));
        if(!p&&head){
            printf("Memory allocation failed!\n");
            freeList(head);
            return NULL;
        }

        printf("Enter the value of Node no.%d\n",i+1);
        scanf("%d",&p->value);
        p->next=NULL;

        if(!head->next)head->next=tail=p;
        else{
            tail->next=p;
            tail=tail->next;
        }
    }

    return head;
}