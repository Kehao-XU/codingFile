#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *fp;
    fp=fopen("c:\\users\\29606\\codingFile\\lessons\\newFile.txt","r");
    if(!fp){
        printf("Open file failed!\n");
        exit(-1);
    }
    
    fseek(fp,0L,SEEK_END);
    printf("%ld",ftell(fp));  // %ld 是 long 类型的格式说明符
    fclose(fp);

    return 0;
}