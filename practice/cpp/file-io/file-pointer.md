# 文件指针变量详解

## 什么是文件指针变量？

文件指针变量是C语言中用于**访问文件**的特殊变量类型。它指向一个**文件结构体**（FILE结构体），该结构体包含了文件的所有信息，如文件位置、读写权限、缓冲区等。

可以将文件指针想象成一个"遥控器"，通过它我们可以控制对文件的各种操作（打开、关闭、读写等）。

## 文件指针的声明

在C语言中，文件指针的类型是`FILE*`，需要包含头文件`<stdio.h>`。

### 语法
```c
FILE *指针名;
```

### 示例
```c
#include <stdio.h>

int main() {
    FILE *fp;  // 声明一个文件指针变量fp
    return 0;
}
```

## 文件指针的初始化（打开文件）

要使用文件指针，必须先将其与实际文件关联（打开文件）。使用`fopen()`函数来完成这个操作。

### fopen()函数
```c
FILE *fopen(const char *filename, const char *mode);
```

- **参数1**：文件名（字符串）
- **参数2**：打开模式（字符串）
- **返回值**：成功返回文件指针，失败返回NULL

### 常用打开模式
| 模式 | 含义 |
|------|------|
| "r"  | 只读模式，打开已存在的文件 |
| "w"  | 只写模式，创建新文件或覆盖已有文件 |
| "a"  | 追加模式，在文件末尾添加内容 |
| "r+" | 读写模式，打开已存在的文件 |
| "w+" | 读写模式，创建新文件或覆盖已有文件 |
| "a+" | 读写模式，在文件末尾添加内容 |

### 示例：打开文件
```c
#include <stdio.h>

int main() {
    FILE *fp;
    
    // 以只读模式打开文件
    fp = fopen("example.txt", "r");
    
    if (fp == NULL) {
        printf("无法打开文件！\n");
        return 1;
    }
    
    printf("文件打开成功！\n");
    
    // 使用文件后要关闭
    fclose(fp);
    
    return 0;
}
```

## 文件关闭操作

使用完文件后，必须用`fclose()`函数关闭文件，释放资源。

### fclose()函数
```c
int fclose(FILE *stream);
```

- **参数**：文件指针
- **返回值**：成功返回0，失败返回EOF

### 示例：关闭文件
```c
#include <stdio.h>

int main() {
    FILE *fp = fopen("example.txt", "w");
    
    if (fp == NULL) {
        printf("无法打开文件！\n");
        return 1;
    }
    
    // 进行文件操作...
    
    // 关闭文件
    if (fclose(fp) == 0) {
        printf("文件关闭成功！\n");
    } else {
        printf("文件关闭失败！\n");
    }
    
    return 0;
}
```

## 文件读写操作

### 1. 字符读写

#### 读取单个字符：fgetc()
```c
int fgetc(FILE *stream);
```
- 返回值：成功返回读取的字符（ASCII码），失败或文件结束返回EOF

#### 写入单个字符：fputc()
```c
int fputc(int character, FILE *stream);
```
- 参数1：要写入的字符（ASCII码）
- 返回值：成功返回写入的字符，失败返回EOF

#### 示例：字符读写
```c
#include <stdio.h>

int main() {
    FILE *fp;
    char ch;
    
    // 写入文件
    fp = fopen("example.txt", "w");
    if (fp == NULL) return 1;
    
    fputc('H', fp);
    fputc('e', fp);
    fputc('l', fp);
    fputc('l', fp);
    fputc('o', fp);
    fclose(fp);
    
    // 读取文件
    fp = fopen("example.txt", "r");
    if (fp == NULL) return 1;
    
    printf("读取的内容：");
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }
    printf("\n");
    
    fclose(fp);
    return 0;
}
```

### 2. 字符串读写

#### 读取字符串：fgets()
```c
char *fgets(char *str, int n, FILE *stream);
```
- 参数1：存储字符串的缓冲区
- 参数2：最多读取的字符数（包括'\0'）
- 参数3：文件指针
- 返回值：成功返回str，失败或文件结束返回NULL

#### 写入字符串：fputs()
```c
int fputs(const char *str, FILE *stream);
```
- 参数1：要写入的字符串
- 参数2：文件指针
- 返回值：成功返回非负值，失败返回EOF

#### 示例：字符串读写
```c
#include <stdio.h>

int main() {
    FILE *fp;
    char buffer[100];
    
    // 写入字符串
    fp = fopen("example.txt", "w");
    if (fp == NULL) return 1;
    
    fputs("Hello, World!\n", fp);
    fputs("This is a test file.\n", fp);
    fclose(fp);
    
    // 读取字符串
    fp = fopen("example.txt", "r");
    if (fp == NULL) return 1;
    
    printf("读取的内容：\n");
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    
    fclose(fp);
    return 0;
}
```

### 3. 格式化读写

#### 格式化读取：fscanf()
```c
int fscanf(FILE *stream, const char *format, ...);
```
- 类似于scanf()，但从文件中读取

#### 格式化写入：fprintf()
```c
int fprintf(FILE *stream, const char *format, ...);
```
- 类似于printf()，但写入到文件中

#### 示例：格式化读写
```c
#include <stdio.h>

int main() {
    FILE *fp;
    char name[20];
    int age;
    float score;
    
    // 写入格式化数据
    fp = fopen("students.txt", "w");
    if (fp == NULL) return 1;
    
    fprintf(fp, "%s %d %.2f\n", "Alice", 18, 95.5);
    fprintf(fp, "%s %d %.2f\n", "Bob", 19, 88.0);
    fclose(fp);
    
    // 读取格式化数据
    fp = fopen("students.txt", "r");
    if (fp == NULL) return 1;
    
    printf("学生信息：\n");
    while (fscanf(fp, "%s %d %f", name, &age, &score) != EOF) {
        printf("姓名：%s，年龄：%d，分数：%.2f\n", name, age, score);
    }
    
    fclose(fp);
    return 0;
}
```

## 文件定位操作

文件指针会自动移动，但我们也可以手动控制它的位置。

### ftell()：获取当前位置
```c
long ftell(FILE *stream);
```
- 返回值：当前文件位置（从文件开头的字节数）

### fseek()：设置文件位置
```c
int fseek(FILE *stream, long offset, int whence);
```
- 参数1：文件指针
- 参数2：偏移量（字节数）
- 参数3：起始位置
  - SEEK_SET：文件开头
  - SEEK_CUR：当前位置
  - SEEK_END：文件结尾

### rewind()：回到文件开头
```c
void rewind(FILE *stream);
```
- 将文件指针重置到文件开头

#### 示例：文件定位
```c
#include <stdio.h>

int main() {
    FILE *fp;
    long position;
    
    fp = fopen("example.txt", "r");
    if (fp == NULL) return 1;
    
    // 读取2个字符
    fgetc(fp);
    fgetc(fp);
    
    // 获取当前位置
    position = ftell(fp);
    printf("当前位置：%ld\n", position);  // 输出：2
    
    // 向前移动1个位置
    fseek(fp, 1, SEEK_CUR);
    printf("移动后位置：%ld\n", ftell(fp));  // 输出：3
    
    // 回到文件开头
    rewind(fp);
    printf("回到开头后位置：%ld\n", ftell(fp));  // 输出：0
    
    fclose(fp);
    return 0;
}
```

## 文件指针的错误处理

在使用文件指针时，必须始终检查操作是否成功：

### 1. 检查文件是否成功打开
```c
FILE *fp = fopen("file.txt", "r");
if (fp == NULL) {
    printf("无法打开文件！\n");
    return 1;  // 或其他错误处理
}
```

### 2. 检查读写操作是否成功
```c
if (fputc('A', fp) == EOF) {
    printf("写入失败！\n");
}
```

### 3. 检查文件是否成功关闭
```c
if (fclose(fp) != 0) {
    printf("文件关闭失败！\n");
}
```

## 常见问题和注意事项

### 1. 忘记关闭文件
- **问题**：可能导致资源泄漏、数据丢失（缓冲区未刷新）
- **解决方法**：始终使用fclose()关闭不再使用的文件

### 2. 文件打开模式错误
- **问题**：尝试以只读模式写入文件
- **解决方法**：根据需要选择正确的打开模式

### 3. 缓冲区问题
- **问题**：写入的数据可能暂时存储在缓冲区，未立即写入磁盘
- **解决方法**：使用fflush()强制刷新缓冲区

```c
fprintf(fp, "Hello");
fflush(fp);  // 立即将数据写入磁盘
```

### 4. 指针悬空
- **问题**：文件关闭后仍使用文件指针
- **解决方法**：关闭文件后将指针设为NULL

```c
fclose(fp);
fp = NULL;  // 防止悬空指针
```

## 完整示例程序

下面是一个完整的文件操作示例，演示了文件的创建、写入、读取和关闭：

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char buffer[100];
    
    // 1. 创建并写入文件
    fp = fopen("my_file.txt", "w");
    if (fp == NULL) {
        printf("创建文件失败！\n");
        exit(1);
    }
    
    printf("请输入一些文本（输入EOF结束，Windows下按Ctrl+Z）：\n");
    
    // 读取用户输入并写入文件
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fputs(buffer, fp);
    }
    
    fclose(fp);
    printf("文件写入完成！\n\n");
    
    // 2. 读取并显示文件内容
    fp = fopen("my_file.txt", "r");
    if (fp == NULL) {
        printf("打开文件失败！\n");
        exit(1);
    }
    
    printf("文件内容如下：\n");
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    
    fclose(fp);
    printf("\n文件读取完成！\n");
    
    return 0;
}
```

## 总结

文件指针变量是C语言中访问文件的核心工具：

1. **声明**：`FILE *指针名;
2. **打开**：使用`fopen()`函数关联实际文件
3. **操作**：
   - 字符读写：`fgetc()`/`fputc()`
   - 字符串读写：`fgets()`/`fputs()`
   - 格式化读写：`fscanf()`/`fprintf()`
4. **定位**：`ftell()`/`fseek()`/`rewind()`
5. **关闭**：使用`fclose()`释放资源

始终记住：**打开的文件必须关闭，操作必须检查错误**！

## 实践建议

1. 从简单的程序开始练习（如字符读写）
2. 逐步尝试更复杂的操作（格式化读写、文件定位）
3. 注意错误处理，养成良好的编程习惯
4. 尝试编写实际应用（如文本编辑器、数据记录程序）

通过不断练习，你会逐渐掌握文件指针的使用技巧！