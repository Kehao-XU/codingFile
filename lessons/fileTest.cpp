#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *fp;
    fp=fopen("c:\\users\\29606\\codingFile\\lessons\\newFile.txt","w");
    if(!fp){
        printf("Open file failed!\n");
        exit(-1);
    }
    
    char s[100];
    printf("Enter your string.\n");
    fgets(s,sizeof(s),stdin);
    fputs(s,fp);
    fclose(fp);

    return 0;
}