# 常见的File函数
## fopen()
```cpp
FILE *fopen(char *fname,char *mode)
```
fname可以输入**绝对路径**，也可以输入**相对路径**。
mode可以输入的字符详见`C语言程序设计第三版`P189。

当文件可以打开时，会返回文件地址，当文件不能打开的时候，这个函数会返回NULL。

用“r”方式只能打开已经存在的文件，用“w”方式则可以创建并更改文件；如果w的文件已经存在，那么会清除文件上的所有数据。

对于正文文件，输入时，回车符和换行符合成为一个换行符输入；输出时，换行符转换成为回车符和换行符两个字符一起输出。

stdin,stdout,stderr和stdprn也都是文件，可以在使用函数的时候输入这些变量表示从标准输入输出设备输入输出数据。例如对stdin调用输入库函数就表示从键盘输入数据，对stdout调用输出库函数就是向显示屏输出数据。

## fclose()
```cpp
int fclose(FILE *fp)
```
调用该函数的作用是让文件指针变量终止早先调用函数fopen()时所建立的与文件的联系。如果关闭成功返回0；如果检测到错误返回EOF(End of File)。

及时调用fclose()还有防止数据丢失的重要作用。fclose()会把暂留在缓冲区中的数据输出到文件中，然后才终止文件指针变量与文件之间的联系。

## fgetc()
```cpp
int fgetc(FILE *fp)
```
从与fp相联系的文件中输入当前位置上的一个字符。每输入一个字符后，在文件还未结束的情况下，这个当前位置就会向后一个字符位置，从而保证程序反复调用这个函数可以顺序输入文件中的所有字符。

这个函数的返回值是输入字符的ASCII码。遇到文件结束就会返回EOF(通常是-1)。

## fputc()
```cpp
int fputc(char ch,FILE *fp)
```
该函数的功能是将ch中用ASCII码表示的字符输出到与fp文件指针相联系的文件中。如果输出成功，fputc()返回输出字符的ASCII码；如果输出失败，则返回EOF。

## fprintf()&fscanf()
```cpp
fprintf(wp,输出格式控制字符串，输出项表);
fscanf(rp,输入格式控制字符串，输入项地址表);
```

## fgets()&fputs()
```cpp
char *fgets(char *str,int n.FILE *fp)
int fputs(char *str,FILE *fp)
```
fgets()用于从数据文件输入字符序列，并存于字符指针所指出的存储区域中。当连续输入n-1个字符，或遇到换行符，或遇到文件结束时，输入字符过程结束。

fgets()和gets()都会在输入的字符序列后自动存储字符串结束标识符'\0'，但是fgets()还会存储输入的换行符。

fputs()的作用是将由参数所指的字符串输出到文件。

## rewind(),fseek()&ftell()
```cpp
void rewind(FILE *fp)
```
其作用是使文件当前位置重新回到文件之首。
```cpp
fseek(FILE *fp,long offset,int ptrname)
```
这个函数中ptrname表示定位基准，只允许0、1、2。其中，0代表以文件首为基准，1代表以当前位置为基准，2代表以文件尾为基准。