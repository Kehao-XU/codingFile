#include<stdio.h>
struct studentNode{
    char name[10];
    int number;
    float mathMark;
    float engMark;
};

void createClass(studentNode *Class,int n){
    for(int i=0;i<n;i++){
        printf("Enter the name of student no.%d\n",i+1);
        scanf("%s",Class[i].name);
        printf("Enter the number of student no.%d\n",i+1);
        scanf("%d",&Class[i].number);
        printf("Enter the mathMark of student no.%d\n",i+1);
        scanf("%f",&Class[i].mathMark);
        printf("Enter the engMark of student no.%d\n",i+1);
        scanf("%f",&Class[i].engMark);
    }
}

void avrSolve(studentNode* Class,int n){
    float avr[n];
    for(int i=0;i<n;i++){
        avr[i]=(Class[i].mathMark+Class[i].engMark)/2;
        printf("The average of Student no.%d is %.2f\n",i+1,avr[i]);
    }
    float max=0;
    int maxone=-1;
    for(int i=0;i<n;i++){
        if(avr[i]>max){
            max=avr[i];
            maxone=i;
        }
    }
    if(maxone!=-1){
        printf("The name of the best student is %s,\
        his number is %d,his mathMark is %.2f,his engMark is %.2f\n",\
        Class[maxone].name,Class[maxone].number,Class[maxone].mathMark,Class[maxone].engMark);
    }
}

int main(){
    int n;
    printf("Enter the number of students.\n");
    scanf("%d",&n);
    studentNode Class[n];
    createClass(Class,n);
    avrSolve(Class,n);
    return 0;
}