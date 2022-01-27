/*
    ------------
    | 凯撒密码 |
    ------------
注意点:
    一、需要命令行参数输入，除文件名外，只需要一个参数key（偏移度）；
    二、只对字母表内的大小写字母加密，不理睬除字母表字母以外的符号；
    三、提示文件的使用，如果用户不配合的话；
*/
/*
算法：
    一、判断用户是否IsRight
        二、若不，则打印“Usage ./caesar key”
        三、若是，则跳转到四；
    四、从命令行参数中提取key，并将其从字符串转换到整数；
    五、打印“plaintext：”并等待用户输入；
    六、调用encrypt函数实现加密，并返回成功1或0；
    七、打印“ciphertext：%s”的加密字符串；

int Encrypt（char *string, key);
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>                           // 编译时注意用-lcs50链接就好了

#define Error   0
#define Pass    1

void Encrypt(char *string, int key);        // 加密函数，传入两个参数，前者是明文，后者是偏移度
int IsRight(int argc, char **argv);         // 判断用户命令行输入是否有效，判断输入参数的数量、类型符不符合
long StrToInt(char *string);                // 将Key从字符串转换到整数，传入参数实际上是argv[1]

int main(int argc, char *argv[])
{
    long key = 0;
    //char string[100];                             // 能尽量不用char *str就不要用这种方法，采取这种方法进行的赋值默认为一种常量
    char *string;                                   // 也就是说，一旦开始赋值了，那你就不可能修改str的值了，这和Encrypt函数相悖。 

    if( !IsRight(argc, argv) )
    {
        printf("Usage: ./caesar key\n");
        exit(EXIT_FAILURE);
    }
    
    key = StrToInt(argv[1]);

    //printf("plaintext: ");
    //scanf("%s", string);
    string = get_string("plaintext: ");             // 用cs50.h的get_string可以避免scanf读取时考虑格式化的问题 
    Encrypt(string, key);
    printf("ciphertext: %s\n", string);

    return 0;
}

void Encrypt(char *string, int key)
{
    int i = 0;

    while(string[i] != '\0')
    {
        if (string[i] > 'a' && string[i] < 'z')
        {
            string[i] = ( string[i] + key - 'a' ) % 26 + 'a';           // string[i]加上key后相比于'a'偏移程度
        }
        else if (string[i] > 'A' && string[i] < 'Z')
        {
            string[i] = ( string[i] + key - 'A' ) % 26 + 'A';
        }
        i++;
    }
}

int IsRight(int argc, char **argv)
{
    int j = 0;
    if (argc == 2)                                                  // 判断是否读入除文件名外的参数只有一个
    {
        if (strlen(argv[1]))                                        // 如果不输入参数，则退出；输入参数的长度不为零则进行下面的判断
        {
            while (argv[1][j])                                      // 遍历字符串内的每一个字符，只要有一个不是0~9那么返回错误0；
            {
                if (argv[1][j] > '9' || argv[1][j] < '0') return Error;
                j++;
            }
            return Pass;
        }
        else
        {
            return Error;
        }
    }
    else
    {
        return Error;        
    }
}

long StrToInt(char *string)
{
    int cnt = 0;
    long key = 0;
    while (string[cnt])                         // 计数器，只要字符串没有检索到'\0'
    {
        key = key * 10 + (string[cnt] - '0');   // 从左往右取，先取出来的应该是最大位，所以每一次循环都乘10来增加位数
        cnt++;
    }
    
    return key;
    
}