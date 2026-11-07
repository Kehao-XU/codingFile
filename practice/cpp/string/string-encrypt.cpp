#include<stdio.h>
#include<malloc.h>
#include<string.h>

struct charNode{
    short status;
    char letter;
    charNode* next;
};

void freeList(charNode* head){
    charNode* temp=head;
    while(head){
        head=head->next;
        free(temp);
        temp=head;
    }
}

charNode* encrypt(int key,char* s){
    //get the length of s
    int n=strlen(s);

    //create a linkedList recording s, set every node's status to 1
    charNode *head=NULL,*tail=NULL,*p=NULL;
    for(int i=0;i<n;i++){
        p=(charNode*)malloc(sizeof(charNode));
        if(p==NULL){
            printf("Memory allocation failed!\n");
            freeList(head);
            return NULL;
        }

        //initialize p
        p->letter='0';
        p->status=1;
        p->next=NULL;

        //append p to the linkedList
        if(!head){
            head=p;
            tail=p;
            p=NULL;
        }
        else{
            tail->next=p;
            tail=tail->next;
            p=NULL;
        }
    }

    //make the linkedList a cycle
    tail->next=head;

    //encrypt the string according to the key
    p=head;
    for(int i=0;i<n;i++){
        int j=0;
        while(j<key){
            while(!p->next->status){
                p=p->next;
            }
            p=p->next;
            j++;
        }
        p->letter=s[i];
        p->status=0;
    }

    //break the cycle and make it a singlyLinkedList
    tail->next=NULL;

    return head;
}

void printList(charNode *head){
    charNode *p=head;
    printf("\n");
    while(p){
        printf("%c",p->letter);
        p=p->next;
    }
    printf("\n");
}

int main(){
    char s[20];
    printf("Enter a string.\n");
    scanf("%s",s);

    int key;
    printf("\nEnter a key.\n");
    scanf("%d",&key);

    charNode *head=encrypt(key,s);
    printList(head);
    freeList(head);

    return 0;
}