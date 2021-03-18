#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools/inc/utstack.h"

/*
速记内容复原
    工作级第3题/专业级第1题
有一种速记方式，针对重复内容有一套独特的缩写规则：
    重复的部分会被以 "(重复内容)<重复次数>" 形式记录，并且可能存在嵌套缩写关系。不重复的部分按照原样记录。
    现给一个符合此速记方式的字符串 records，请以字符串形式返回复原后的内容。
注： records 仅由小写字母、数字及<, >, (, )组成。
示例 1：
    输入：records = "abc(d)<2>e"
    输出："abcdde"
    解释：字符串中出现 "(d)<2>"，表示 "d" 重复出现 2 次，因此返回复原后的内容 "abcdde"。
*/

#define MAX_LEN 201

typedef struct el {
    char currStr[MAX_LEN];
    struct el *next;
} el;

int GetRepeatNum(char* str)
{
    char num[MAX_LEN] = {0};
    for (int i = 0; str[i] != '>'; i++) {
        num[i] = str[i];
    }
    return atoi(num);
}

char* UnzipString(char* records)
{
    char *head = records;
    char *curr = (char *)calloc(MAX_LEN, sizeof(char));
    char tmp[MAX_LEN] = {0}; 
    el *stack = NULL;
    el *stackTop = NULL;
    int repeatNum = 0;
    int i = 0;

    for (; *head != '\0'; head++) {
        if (*head == '(') {         // 左括号，将curr压栈
            stackTop = (el *)malloc(sizeof(el));
            strcpy(stackTop->currStr, curr);
            STACK_PUSH(stack, stackTop);
            memset(curr, 0, sizeof(curr));
            i = 0;
        } else if (*head == ')') {  // 右括号，弹出栈顶到tmp，重复repeatNum次curr
            repeatNum = GetRepeatNum(head + 2);
            STACK_POP(stack, stackTop);
            strcpy(tmp, stackTop->currStr);
            free(stackTop);
            for (int j = 0; j < repeatNum; j++) {
                strcat(tmp, curr);
            }
            strcpy(curr, tmp);
            i = strlen(curr);
            head += 3;
        } else {                    // 字母直接追加到curr
            sprintf(curr + i++, "%c", *head);
        }
    }
    return curr;
}


#include "tools/inc/test.h"
int main()
{
    TEST_FUNC_BEGIN();

    char *ret = NULL;
    
    ret = UnzipString("abc(d)<2>e");
    STR_MATCH(ret, "abcdde");
    printf("%s\n", ret);
    free(ret);

    ret = UnzipString("a(b(c)<3>d)<2>e");
    STR_MATCH(ret, "abcccdbcccde");
    printf("%s\n", ret);
    free(ret);

    TEST_FUNC_END();
    system("pause");
    return 0;
}