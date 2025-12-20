#include <stdio.h>

int main() {
    FILE *fp1 = fopen("../file1.txt", "r");
    FILE *fp2 = fopen("../file2.txt", "r");
    FILE *fout = fopen("../merged.txt", "w");

    if (fp1 == NULL || fp2 == NULL || fout == NULL) {
        printf("文件打开失败！\n");
        return 1;
    }

    int num1, num2;
    int flag1 = fscanf(fp1, "%d", &num1);
    int flag2 = fscanf(fp2, "%d", &num2);
    int last_written;
    int first_write = 1;

    while (flag1 != EOF && flag2 != EOF) {
        if (num1 <= num2) {
            if (first_write || num1 != last_written) {
                fprintf(fout, "%d\n", num1);
                last_written = num1;
                first_write = 0;
            }
            flag1 = fscanf(fp1, "%d", &num1);
        } else {
            if (first_write || num2 != last_written) {
                fprintf(fout, "%d\n", num2);
                last_written = num2;
                first_write = 0;
            }
            flag2 = fscanf(fp2, "%d", &num2);
        }
    }

    while (flag1 != EOF) {
        if (first_write || num1 != last_written) {
            fprintf(fout, "%d\n", num1);
            last_written = num1;
            first_write = 0;
        }
        flag1 = fscanf(fp1, "%d", &num1);
    }

    while (flag2 != EOF) {
        if (first_write || num2 != last_written) {
            fprintf(fout, "%d\n", num2);
            last_written = num2;
            first_write = 0;
        }
        flag2 = fscanf(fp2, "%d", &num2);
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fout);

    printf("合并完成！\n");
    return 0;
}