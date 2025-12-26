/*设计一个程序来处理一元多项式的加法和乘法。每个多项式由若干个项组成，每个项包括：

系数（浮点数）

指数（非负整数）

多项式按指数降序排列在链表中。

要求实现以下功能：

创建多项式：从键盘输入，每行输入一个项的系数和指数（以0 0结束）

多项式加法：将两个多项式相加，返回结果多项式

多项式乘法：将两个多项式相乘，返回结果多项式

输出多项式：格式化输出多项式（如 3.5x^2 + 2.1x - 1.0）

销毁多项式：释放所有内存*/
#include<stdio.h>
#include<malloc.h>
struct Term {
    float coef;      // 系数
    int exp;         // 指数
    struct Term *next;
};

typedef struct Term* Polynomial;

Polynomial create_polynomial(){
    Polynomial Poly=(Term*)malloc(sizof(Term)),Tail,p;
    if(!Poly){
        printf("Memory allocation failed!\n");
        return NULL;
    }
    Poly->exp=-1;
    Poly->coef=-1;
    Poly->next=NULL;
    Tail=Poly;

    int exponent;
    float coefficient;
    do{
        scanf("%f",&coefficient);
        scanf("%d",&exponent);

        if((coefficient-0)>1e-6||exponent!=0){
            p=(Term*)malloc(sizeof(Term));
            p->coef=coefficient;
            p->exp=exponent;
            p->next=NULL;
            Tail->next=p;
            Tail=p;
        }
        else break;
    }
    while(True);

    return Poly;
}

Polynomial add_polynomial(Polynomial p1, Polynomial p2);
Polynomial multiply_polynomial(Polynomial p1, Polynomial p2);
void print_polynomial(Polynomial p);
void destroy_polynomial(Polynomial p);