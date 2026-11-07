#include<stdio.h>
int main(){
    int a[10];
    for(int i=0;i<10;i++){
        printf("Enter the value of No.%d\n",i+1);
        scanf("%d",&a[i]);
    }

    int sum=0;
    double avr=0;
    for(int i=0;i<10;i++){
        sum+=a[i];
    }
    avr=(double)sum/10.0;
    printf("Sum is %d,average is %.2lf",sum,avr);
    return 0;
}