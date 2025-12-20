#include<stdio.h>
#include<time.h>
#include<stdlib.h>
int main(){
    srand(time(NULL));
    FILE *fp=fopen("../testT6.txt","w+");  // 使用w+模式，允许同时读写
    if(!fp){
        printf("File open failed!\n");
        return 0;
    }

    int limit=1+rand()%20;
    for(int i=0;i<limit;i++){
        fprintf(fp,"%d\n",1+rand()%100);
    }

    rewind(fp);

    int num,max=0,min=100;
    for(int i=0;i<limit;i++){
        fscanf(fp,"%d",&num);
        printf("%d\n",num);
        if(num>max)max=num;
        if(num<min)min=num;
    }
    printf("There are %d numbers in this file,max is %d,min is %d.",limit,max,min);

    fclose(fp);
    return 0;
}