#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tools/inc/uthash.h"
#include "tools/inc/test.h"


/*
设计通讯录
    工作级第2题
请你设计一个简易通讯录系统，实现以下功能：
Add(String name, String number) 增加联系人：名字为 name，联系号码为 number，若已经存在不作处理；
Update(String name, String number) 更新联系人信息：若系统中已存在联系人 name，则将此联系人的联系号码更新为 number，
                                                   并返回其更新前的联系号码；若系统中不存在此联系人，则不作处理并返回 "No such person"；
Delete(String name) 删除联系人：若系统中已存在联系人 name，则在系统中删除此联系人记录、并返回其联系号码；
                                若系统中不存在此联系人，则返回 "No such person"；
Prefix(String s) 过滤联系人：把名字前缀为 s的联系人过滤出来（区分大小写），并把记录按照名字的字典序返回，
                             形如 [["Abby","12321"],["Alice","1-22442"]...]。
*/

#define MAX_PHONE   1000
#define COLSIZE     2
#define MAX_LENGTH  20

typedef struct {
    char *name;         // key
    char *number;
    UT_hash_handle hh;  // makes this structure hashable
} PhoneSystem;

PhoneSystem* PhoneSystemCreate()
{
    PhoneSystem *obj = NULL;    // important! initialize to NULL
    PhoneSystem *node = (PhoneSystem *)malloc(sizeof(PhoneSystem));

    node->name = "0";
    node->number = "666";
    HASH_ADD_STR(obj, name, node);
    return obj;
}

void PhoneSystemAdd(PhoneSystem* obj, char* name, char* number)
{
    PhoneSystem *node = NULL;

    HASH_FIND_STR(obj, name, node);  // name already in the hash?
    if(!node) {
        node = (PhoneSystem *)malloc(sizeof(PhoneSystem));
        node->name = name;
        node->number = number;
        HASH_ADD_STR(obj, name, node);
    }
}

char* PhoneSystemUpdate(PhoneSystem* obj, char* name, char* number)
{
    PhoneSystem *node = NULL;

    HASH_FIND_STR(obj, name, node);  // name already in the hash?
    if(node) {
        char *tmp = node->number;
        node->number = number;
        return tmp;
    }

    return "No such person";
}

char* PhoneSystemDelete(PhoneSystem* obj, char* name)
{
    PhoneSystem *node = NULL;

    HASH_FIND_STR(obj, name, node);  // name already in the hash?
    if(node) {
        char *tmp = node->number;
        HASH_DEL(obj, node);
        return tmp;
    }

    return "No such person";
}

int CompStr(const void *a, const void *b)
{
    return strcmp((*(PhoneSystem*)a).name, (*(PhoneSystem*)b).name);
}

int name_sort(PhoneSystem* phoneA, PhoneSystem* phoneB)
{
    return strcmp(phoneA->name, phoneB->name);
}

// *retSize: 前缀为s的联系人数量
// **retColSize: 2，[name, number]
// *retColSize: ret的行，实际为*retSize，分配时设为MAX_PHONE
char*** PhoneSystemPrefix(PhoneSystem* obj, char* s, int* retSize, int** retColSize)
{
    char ***ret = (char ***)malloc(sizeof(char **) * MAX_PHONE);
    *retColSize = (int *)malloc(sizeof(int) * MAX_PHONE);
    *retSize = 0;

    PhoneSystem* curPhone = NULL;
    PhoneSystem *tmp = NULL;
    HASH_SORT(obj, name_sort);
    HASH_ITER(hh, obj, curPhone, tmp) {
        if (strncmp(s, curPhone->name, strlen(s)) == 0) {
            ret[*retSize] = (char **)malloc(sizeof(char *) * COLSIZE);
            ret[*retSize][0] = curPhone->name;
            ret[*retSize][1] = curPhone->number;
            (*retColSize)[*retSize] = COLSIZE;
            (*retSize)++;
        }
    }
    return ret;
}

void PhoneSystemFree(PhoneSystem* obj)
{
    PhoneSystem *curPhone = NULL;
    PhoneSystem *tmp = NULL;

    HASH_ITER(hh, obj, curPhone, tmp) {
        HASH_DEL(obj, curPhone);    // delete it (users advances to next)
        free(curPhone);             // free it
    }
}

int main()
{
    TEST_FUNC_BEGIN();

    // test1
    PhoneSystem* obj = PhoneSystemCreate();

    PhoneSystemAdd(obj, "Herry", "123");
    PhoneSystemAdd(obj, "Merry", "456");

    char *oldName = PhoneSystemUpdate(obj, "Merry", "789");
    SHOULD(strcmp(oldName, "456") == 0);

    int retSize;
    int* retColSize;
    char*** nameList = PhoneSystemPrefix(obj, "Me", &retSize, &retColSize);
    SHOULD(strcmp(nameList[0][0], "Merry") == 0);
    SHOULD(strcmp(nameList[0][1], "789") == 0);
    for (int i = 0; i < retSize; i++) {
        free(nameList[i]);
    }
    free(retColSize);
    free(nameList);

    oldName = PhoneSystemDelete(obj, "Merry");
    SHOULD(strcmp(oldName, "789") == 0);

    PhoneSystemAdd(obj, "Herry", "234");
    PhoneSystemAdd(obj, "Henry", "2564");

    oldName = PhoneSystemUpdate(obj, "Merry", "111");
    SHOULD(strcmp(oldName, "No such person") == 0);

    nameList = PhoneSystemPrefix(obj, "He", &retSize, &retColSize);
    SHOULD(strcmp(nameList[0][0], "Henry") == 0);
    SHOULD(strcmp(nameList[0][1], "2564") == 0);
    SHOULD(strcmp(nameList[1][0], "Herry") == 0);
    SHOULD(strcmp(nameList[1][1], "123") == 0);
    for (int i = 0; i < retSize; i++) {
        free(nameList[i]);
    }
    free(retColSize);
    free(nameList);

    TEST_FUNC_END();

    system("pause");
    return 0;
}