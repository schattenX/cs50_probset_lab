#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

int count_letters(char *string);                // 计算出现字符的数量
int count_words(char *string);                  // 计算出现单词的数量
int count_sentences(char *string);              // 计算出现句子的数量
int Gloeman_Liau_Index(int letr_cnt, int wrd_cnt, int sentce_cnt);      // index = 0.0588 * L - 0.296 * S - 15.8

int main(void)
{
    char *text;
    int letr_cnt, wrd_cnt, sentce_cnt, index;
    // 使用户输入文本
    text = get_string("Text: ");

    letr_cnt = count_letters(text);
    wrd_cnt = count_words(text);
    sentce_cnt = count_sentences(text);
    
    index = Gloeman_Liau_Index(letr_cnt, wrd_cnt, sentce_cnt);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }

    //printf("%s\n", text);
    // printf("\n字符数量为：%d\n", count_letters(text));
    // printf("单词数量为：%d\n", count_words(text));
    // printf("句子数量为：%d\n", count_sentences(text));

    return 0;
}

int count_letters(char *string)
{
    // 计算字符的数量
    int i = 0;
    int lcnt = 0;                       // 字母计数器
    while (string[i] != '\0')
    {
        if ( (string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z') )
        {
            // printf("%c ", string[i]);
            lcnt++;
        }
        i++;
    }

    return lcnt;
}

int count_words(char *string)
{
    // 计算单词的数量---由空格分隔的连续字母串为一个单词
    int i = 0;
    int lcnt = 0;                       // 字母计数器
    int wcnt = 0;                       // 单词计数器
    while (string[i] != '\0')
    {
        
        // 判断是否为一个单词
        if ( (string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z') )
        {
            lcnt++;
            
        }
        else if (string[i] == '\'' || string[i] == '-')     // 考虑由“-”以及“’”连接的单词属于一个单词
        {
            lcnt++;
        }
        else if (lcnt > 0 && string[i] == ' ')
        {
            wcnt++;
            lcnt = 0;
        }
        i++;
    }
    if (lcnt > 0) wcnt++;               // 文本最后以非空格结尾，此行防止最后存在单词但是没检测到发生；

    return wcnt;
}

int count_sentences(char *string)
{
    // 计算句子的数量--只依据句号、感叹号与问好作为分割单位
    int i = 0;
    int scnt = 0;
    while (string[i] != '\0')
    {
        if (string[i] == '.' || string[i] == '!' || string[i] == '?')
        {
            scnt++;
        }
        i++;
    }

    return scnt;
}

int Gloeman_Liau_Index(int letr_cnt, int wrd_cnt, int sentce_cnt)
{
    
    double index = 0;
    double l = (double)letr_cnt / (double)wrd_cnt * 100.0;              // 类型强转
    double s = (double)sentce_cnt / (double)wrd_cnt * 100.0;
    //printf("l=%.2f, s=%.2f\n", l, s);
    index = (0.0588 * l - 0.296 * s -15.8);
    if ((int)(index * 10) % 10 >= 5)                                    // 若第一位小数大于等于5，则四舍五入向上进位
    {
        index++;
    }
    //printf("letr_cnt=%d, wrd_cnt=%d, sentce_cnt=%d, index=%f\n", letr_cnt, wrd_cnt, sentce_cnt, index);
    return (int)index;
}