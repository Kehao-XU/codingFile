#include<stdio.h>
/*题目：编写程序，输入两个整数 a 和 b，计算并输出：

a / b 的整数结果

a / b 的浮点数结果（保留3位小数）

a % b 的结果

要求：考虑 b=0 的情况，给出错误提示。

查漏点：

整数除法 vs 浮点数除法

类型转换（隐式/显式）

除零错误处理*/
#ifdef BasicCalc
int main(){
    int a,b;
    printf("Please enter the value of a&b.\n");
    scanf("%d%d",&a,&b);
    if(!b){
        printf("Cannot divide a by b!\n");
    }
    else{
        printf("a/b=%d\t%.3f\n",a/b,(float)(a/b));
        printf("a%b=%d",a%b);
    }
    return 0;
}
#endif

#ifdef BasicGroup
int max(int a[],int n){
    int max=a[0];
    for(int i=0;i<n;i++){
        if(a[i]>max)max=a[i];
    }
    return max;
}

int min(int a[],int n){
    int min=a[0];
    for(int i=0;i<n;i++){
        if(a[i]<min)min=a[i];
    }
    return min;
}

double average(int a[],int n){
    int sum=0;
    for(int i=0;i<n;i++){
        sum+=a[i];
    }
    return (double)sum/(double)n;
}

void reverseGroup(int a[],int n){
    int *head=&a[0],*tail=&a[n-1];
    for(int i=0;i<n/2;i++){
        int temp;
        temp=*head;*head=*tail;*tail=temp;
        head++;
        tail--;
    }
}

int main(){
    int a[5]={1,2,3,6,5};
    int n=5;
    printf("Max=%d,Min=%d,Average=%lf\n",max(a,n),min(a,n),average(a,n));
    reverseGroup(a,n);
    for(int i=0;i<n;i++){
        printf("%d\t",a[i]);
    }
    return 0;
}
#endif

/* 字符串基本操作
题目：手动实现以下字符串函数（不使用 string.h）：

int my_strlen(char s[]) - 返回字符串长度

void my_strcpy(char dest[], char src[]) - 字符串复制

int my_strcmp(char s1[], char s2[]) - 字符串比较

要求：考虑空字符串、边界条件。

查漏点：

字符串结束符 \0 的处理

数组越界

指针遍历字符串*/
#ifdef BasicString
int my_strlen(char s[]){
    char *temp=s;
    int length=0;
    while(*temp){
        temp++,length++;
    }
    return length;
}

void my_strcpy(char dest[],char src[]){
    char *cur1=dest,*cur2=src;
    while(*cur2) *(cur1++)=*(cur2++);
}

int my_strcmp(char s1[],char s2[]){
    char *cur1=s1,*cur2=s2;
    while(*cur1&&*cur2){
        if(*(cur1++)!=*(cur2++))return 0;
    }
    if(*cur1||*cur2) return 0;
    return 1;
}

int main(){
    char s1[20],s2[20];
    fgets(s1,20*sizeof(char),stdin);
    fgets(s2,20*sizeof(char),stdin);

    printf("Strlen of s1=%d,strlen of s2=%d\n",my_strlen(s1),my_strlen(s2));
    printf("After my_strcpy,s1");
    if(my_strcmp(s1,s2)) printf("=");
    else printf("!=");
    printf("s2");
}
#endif

/*结构体与文件
题目：定义学生结构体（学号、姓名、成绩），实现：

输入5个学生信息

将学生信息保存到文件 students.txt

从文件读取数据，找出成绩最高的学生并输出

要求：使用二进制文件读写，注意文件打开模式。

查漏点：

结构体定义与使用

文件打开/关闭

fread/fwrite 的正确使用*/
#ifdef BasicStructFile
struct StuNode{
    int number;
    char name[20];
    int mark;
};

int main(){
    FILE *fp;
    fp=fopen("../students.txt","wb");
    if(!fp){
        printf("Cannot open file for writing!\n");
        return 1;
    }
    for(int i=0;i<5;i++){
        StuNode student;
        printf("Number:");
        fscanf(stdin,"%d",&student.number);
        printf("Name:");
        fscanf(stdin,"%s",student.name);
        printf("Mark:");
        fscanf(stdin,"%d",&student.mark);
        fwrite(&student,sizeof(StuNode),1,fp);
    }
    fclose(fp); 
    
    // 2. 从文件读取数据
    fp = fopen("../students.txt", "rb");
    if (fp == NULL) {
        printf("Error: Cannot open file for reading!");
        return 1;
    }
    
    StuNode readStudents[5];
    for(int i=0; i<5; i++) {
        fread(&readStudents[i], sizeof(StuNode), 1, fp);
    }
    fclose(fp);
    
    // 3. 找出成绩最高的学生
    int maxIndex = 0;
    for(int i=1; i<5; i++) {
        if (readStudents[i].mark > readStudents[maxIndex].mark) {
            maxIndex = i;
        }
    }
    
    // 4. 输出成绩最高的学生信息
    printf("\n=== 成绩最高的学生信息 ===\n");
    printf("学号: %d\n", readStudents[maxIndex].number);
    printf("姓名: %s\n", readStudents[maxIndex].name);
    printf("成绩: %d\n", readStudents[maxIndex].mark);
    
    return 0;
}
#endif

/*动态内存基础
题目：输入整数 n，动态创建大小为 n 的整数数组，实现：

输入 n 个整数

删除所有偶数（将后续元素前移）

输出剩余元素

释放内存

要求：使用 malloc/free，注意内存泄漏。

查漏点：

动态内存分配与释放

数组元素删除时的移位

NULL 指针检查*/
#define BasicDynamicMemory
#ifdef BasicDynamicMemory
#include<stdlib.h>
int* create_group(int n){
    int *p=(int *)malloc(n*sizeof(int));
    if(!p){
        printf("Memory allocation failed!\n");
        return NULL;
    }

    for(int i=0;i<n;i++,p++){
        fscanf(stdin,"%d",p);
    }

    p-=n;
    return p;
}

void free_group(int* p, int n) {
    free(p);
}

void print_group(int *p,int n){
    for(int i=0;i<n;i++){
        printf("%d ",p[i]);
    }
}

void delete_evenNum(int *p, int *n) {
    int newIndex = 0;
    for(int i=0; i<*n; i++) {
        if(p[i]%2 != 0) {  // 只保留奇数
            p[newIndex++] = p[i];
        }
    }
    *n = newIndex;  // 更新数组大小为保留的元素数量
}  // 修复说明：不应该逐个释放元素，因为malloc分配的是连续内存块，应该整体释放

int main(){
    int n;
    printf("Enter n.\n");
    scanf("%d",&n);

    int *p=create_group(n);
    print_group(p,n);
    delete_evenNum(p,&n);
    print_group(p,n);
    free_group(p,n);

    return 0;
}
#endif