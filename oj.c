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

#define COLSIZE 2
#define MAX_PHONE 1000

typedef struct {
    char *name;         // key
    char *number;
    UT_hash_handle hh;  // makes this structure hashable
} PhoneSystem;

PhoneSystem* PhoneSystemCreate()
{
    PhoneSystem *obj = NULL;    // important! initialize to NULL

    return obj;
}

int Compare(PhoneSystem* phoneA, PhoneSystem* phoneB)
{
    return strcmp(phoneA->name, phoneB->name);
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

bool IsNeedOutput(char* prefix, char* name)//这里其实可以直接用strcmp函数来比较
{
    int nameLen = strlen(name);
    int preLen = strlen(prefix);
    if (nameLen < preLen) {
        return false;
    }
    for (int ix = 0; ix < preLen; ix++) {
        if (prefix[ix] != name[ix]) {
            return false;
        }
    }
    return true;
}

char*** PhoneSystemPrefix(PhoneSystem* obj, char* s, int* retSize, int** retColSize)
{
    
}

void PhoneSystemFree(PhoneSystem* obj)
{
    PhoneSystem *curPhone = NULL, *tmp = NULL;
    HASH_ITER(hh, obj, curPhone, tmp) {
        HASH_DEL(obj, curPhone);    // delete it (users advances to next)
        free(curPhone);             // free it
    }
}