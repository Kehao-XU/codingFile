/*## **题目五：LRU缓存实现（中等/困难）**
**描述**：设计并实现一个LRU（最近最少使用）缓存机制。

**要求**：
- 构造函数 `LRUCache(int capacity)`
- `int get(int key)`：如果key存在则返回值，否则返回-1，并将该key提升为最近使用。
- `void put(int key, int value)`：如果key存在则更新值并提升；否则插入。如果容量满，则淘汰最久未使用的key。

**约束**：
- `get` 和 `put` 操作平均时间复杂度 O(1)。
- 使用**双向链表**和**哈希表**结合实现。

---*/
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<math.h>
// 双向链表节点结构体
typedef struct Node {
    int key;           // 键
    int value;         // 值
    struct Node* prev; // 前驱指针
    struct Node* next; // 后继指针
} Node;

// LRU缓存结构体
typedef struct LRUCache {
    int capacity;       // 缓存容量
    int size;           // 当前缓存大小
    Node* head;         // 双向链表头节点（哨兵）
    Node* tail;         // 双向链表尾节点（哨兵）
    struct Node** hash; // 哈希表，用于快速查找
} LRUCache;

int hash(int key,LRUCache* cache){
    int capacity=cache->capacity;
    return abs(key)%(2*capacity);
}

Node* createNode(int key,int value,LRUCache* cache){
    Node *node=(Node*)malloc(sizeof(Node));
    node->key=key;
    node->value=value;
    node->next=cache->head->next;
    node->prev=cache->head;
    cache->head->next=node;
    node->next->prev=node;
    cache->hash[hash(key)]=node;
    #增加冲突解决机制
    cache->size++;
    return node;
}

LRUCache* createCache(){
    LRUCache *cache=(LRUCache*)malloc(sizeof(LRUCache)) ;
    if(!cache){
        printf("Memory allocation failed!\n");
        return NULL;
    }

    printf("Enter the capacity of LRUCache.\n");
    scanf("%d",&cache->capacity);

    cache->size=0;

    cache->head=(Node*)malloc(sizeof(Node));
    if(!cache->head){
        printf("Memory allocation failed!\n");
        free(cache);
        return NULL;
    }

    cache->tail=(Node*)malloc(sizeof(Node));
    if(!cache->tail){
        printf("Memory allocation failed!\n");
        free(cache->head);
        free(cache);
        return NULL;
    }
    
    cache->head->key=cache->tail->key=-1;
    cache->head->value=cache->tail->value=-1;
    cache->head->next=cache->tail;
    cache->tail->prev=cache->head;
    cache->head->prev=NULL;
    cache->tail->next=NULL;
    
    cache->hash=(Node**)calloc(2*cache->capacity,sizeof(Node*));
    if(!cache->hash){
        printf("Memory allocation failed!\n");
        free(cache->head);
        free(cache->tail);
        free(cache);
        return NULL;
    }

    return cache;
}

int get(int key,LRUCache* cache){
    if(!cache->hash[hash(key,cache)])return -1;

    Node *p=*cache->hash[hash(key,cache)],Head=cache->head;
    p->prev->next=p->next;
    p->next->prev=p->prev;
    p->next=Head->next;
    p->prev=Head;
    Head->next=p;
    p->next->prev=p;

    return p->value;
}

void put(int key, int value,LRUCache* cache){
    int help=get(key,cache);

    if(help!=-1){
        cache->head->next->value=value;
        printf("\nPut value success!\n");
        return;
    }

    if(cache->size<cache->capacity){
        createNode(key,value,cache);
        printf("\nA new node created!\n");
        return;
    }

    if(cache->size==cache->capacity){
        Node *Tail=cache->tail,*LeastUsed=cache->tail->prev,*SecLeast=cache->tail->prev->prev;

        SecLeast->next=Tail;
        Tail->prev=SecLeast;
        cache->hash[hash(LeastUsed->key,cache)]=NULL;
        free(LeastUsed);
        cache->size--;
        
        createNode(key,value,cache);
        printf("\nA new node created!\n");
        return;
    }
}

void PrintCache(LRUCache *cache){
    Node* temp=cache->head->next;
    int time=0;
    if(!temp->next){
        printf("There is none in the cache.\n");
        return;
    }

    while(temp->next){
        printf("No.%d used:key->%d\tvalue->%d\n",time,temp->key,temp->value);
        time++;
        temp=temp->next;
    }
    printf("--------------------\n");
}

void freeCache(LRUCache *cache){
    for(int i=0;i<2*cache->capacity;i++){
        free(cache->hash[i]);
    }

    Node *temp=cache->head,*Head=cache->head;
    while(Head){
        Head=Head->next;
        free(temp);
        temp=Head;
    }

    free(cache);
}

int main(){
    cache=createCache();

    int choice=0,key=0,value=0;
    do{
        printf("\n\nPlease make your choice.\n[1]Get [2]Put [3]PrintCache [4]exit\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:{
                printf("\n\nEnter the key please.\n");
                scanf("%d",&key);
                printf("Your key stores a value of %d.\n",get(key));
                break;
            }
            case 2:{
                printf("\n\nEnter your key and value.\n");
                scanf("%d%d",&key,&value);
                put(key,value);
                break;
            }
            case 3:{
                PrintCache(cache);
                break;
            }
            case 4:break;
            default:printf("Wrong choice!\n");
        }
    }
    while(choice!=4);

    freeCache(cache);
    return 0;
}