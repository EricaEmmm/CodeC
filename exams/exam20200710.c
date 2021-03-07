#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

#define MAX_LEN 501

/*
电话号码转换
    专业级第一题
某语音翻译软件，需要实现如下中英文电话号码转换功能：
    若输入的是英文数字单词或Double组成的电话号码，则输出对应的中文数字单词；
    若输入为中文数字单词组成的电话号码，则输出对应的英文数字单词。
    若输入不合法，则输出字符串ERROR。
    中文数字、英文数字分别见下表：
    中文数字单词：Yi Er San Si Wu Liu Qi Ba Jiu Ling
    英文数字单词：One Two Three Four Five Six Seven Eight Nine Zero
说明：
    输入若存在Double，其后必须跟随英文数字单词，代表两个该数字。如输入DoubleSix，代表SixSix；
    输入保证要么全中文，要么全英文（含Double），并且每个单词都是合法的英文数字单词/中文数字单词/Double；
    输入不合法的场景，仅为Double后跟随的不是英文数字单词。如DoubleLiu非法。
*/

char* StrReplace(char *inputStr, char *oldStr, char *newStr)
{
    char buff[MAX_LEN];
    memset(buff, 0, sizeof(buff));

    for (int i = 0; i < strlen(inputStr); i++) {
        if (!strncmp(inputStr + i, oldStr, strlen(oldStr))) {
            strcat(buff, newStr);
            i += strlen(oldStr) - 1;
        } else {
        	strncat(buff, inputStr + i, 1);
	    }
    }

    strcpy(inputStr, buff);
    return inputStr;
}

bool MatchRegex(const char* pattern, const char* userString)
{
    bool result = false;

    regex_t regex;  // 存放编译后的正则表达式
    int regexInit = regcomp(&regex, pattern, REG_EXTENDED);
    if(regexInit) {
        //Error print : Compile regex failed
    } else {
        int reti = regexec(&regex, userString, 0, NULL, 0);   // 编译正则表达式
        if(reti != REG_NOERROR) {
            //Error print: match failed! 
        } else
        {
            result = true;
        }
    }
    regfree(&regex);  // 释放正则表达式
    return result;
}

void Traslate(const char *input, const char *outBuf, int outBufLen)
{
    if (input[0] == '\0') {
        return;
    }

    char *cnNum[] = {"Yi", "Er", "San", "Si", "Wu", "Liu", "Qi", "Ba", "Jiu", "Ling"};
    char *enNum[] = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Zero"};
    char *doubleEn[] = {"DoubleOne", "DoubleTwo", "DoubleThree", "DoubleFour", "DoubleFive",
                        "DoubleSix", "DoubleSeven", "DoubleEight", "DoubleNine", "DoubleZero"};
    char *normalDoub[] = { "OneOne", "TwoTwo", "ThreeThree", "FourFour", "FiveFive", "SixSix",
                           "SevenSeven", "EightEight", "NineNine", "ZeroZero"};

    // 使用正则表达式匹配
    bool includeDouble = MatchRegex(".*Double.*", input);
    bool legalDouble = MatchRegex(".*Double(One|Two|Three|Four|Five|Six|Seven|Eight|Nine|Zero).*", input);
    //bool includeCn = MatchRegex(".*(Yi|Er|San|Si|Wu|Liu|Qi|Ba|Jiu|Ling).*", input);
    bool includeEn = MatchRegex(".*(One|Two|Three|Four|Five|Six|Seven|Eight|Nine|Zero)+.*", input);

    // 包含Double，但使用非法
    if (includeDouble && !legalDouble) {
        memcpy(outBuf, "ERROR", outBufLen);
        return;
    }

    if (includeEn) {
        for (int i = 0; i < 10; i++) {
            StrReplace(input, doubleEn[i], normalDoub[i]);  // 处理Double+英文数字
            StrReplace(input, enNum[i], cnNum[i]);
        }
    } else {
        for (int i = 0; i < 10; i++) {
            StrReplace(input, cnNum[i], enNum[i]);
        }
    }

    memcpy(outBuf, input, outBufLen);
}




#include "tools/inc/test.h"
int main()
{
    TEST_FUNC_BEGIN();

    // test1
    char outBuf[MAX_LEN];

    char input[] = "SixOneThreeOneDoubleZero";
    Traslate(input, outBuf, 19);
    STR_MATCH(outBuf, "LiuYiSanYiLingLing");

    strcpy(input, "YiLingSanSanJiu");
    Traslate(input, outBuf, 22);
    STR_MATCH(outBuf, "OneZeroThreeThreeNine");

    strcpy(input, "DoubleLiu");
    Traslate(input, outBuf, 22);
    STR_MATCH(outBuf, "ERROR");


    printf("%s\n", "hello");
    TEST_FUNC_END();

    system("pause");
    return 0;
}