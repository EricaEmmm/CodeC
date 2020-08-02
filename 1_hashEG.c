#include <stdio.h>
#include <stdlib.h>
#include "include/hhash2.h"

void kvIntPrint(int k, uintptr_t v, void *arg)
{
    printf("k=%d, v=%d\n", k, v);
}

void IntHashTest()
{
    int ret;
    bool exist;
    uintptr_t v;
    // 初始化 HashTable
    IntHash *IntHashTable = IntHashCreate();
    // 判断 HashTable 是否为空
    printf("HashTable is enmty? %d\n", IntHashEmpty(IntHashTable));
    // 增加 k-v 对，已经存在则刷新 v
    ret = IntHashSet(IntHashTable, 1, 20);
    ret = IntHashSet(IntHashTable, 2, 10);
    // 查询 k 是否存在
    printf("k=1 exist? %d\n", IntHashExist(IntHashTable, 1));
    // 由 k 查 v
    v = IntHashGet(IntHashTable, 1, &exist);
    printf("k=1 exist? %d and v=%d\n", exist, v); 
    // HashTable 长度
    printf("HashTable length %d\n", IntHashSize(IntHashTable));
    // 指定 k 删除 k-v 对
    IntHashDel(IntHashTable, 1);
    IntHashDel(IntHashTable, 2);
    // 查询 k 是否存在
    printf("k=1 exist? %d\n", IntHashExist(IntHashTable, 1));
    /* 数组hash，迭代打印 */
    int num[] = {1, 1, 1, 1, 2, 2, -2, -3};
    for (int i = 0; i < sizeof(num) / sizeof(num[0]); i++)
    {
        v = IntHashGet(IntHashTable, num[i], &exist);
        if (exist)
            IntHashSet(IntHashTable, num[i], v + 1);
        else
            IntHashSet(IntHashTable, num[i], 1);
    }
    IntHashIterFunc fun = kvIntPrint;
    IntHashIterate(IntHashTable, fun, NULL);
    // 删除 HashTable
    IntHashDestroy(IntHashTable);
}

void kvStrPrint(const char *k, uintptr_t v, void *arg)
{
    printf("k=%s, v=%d\n", k, v);
}

void StrHashTest()
{
    int ret;
    bool exist;
    uintptr_t v;
    // 初始化 HashTable
    IntHash *StrHashTable = StrHashCreate();
    // 判断 HashTable 是否为空
    printf("HashTable is enmty? %d\n", StrHashEmpty(StrHashTable));
    // 增加 k-v 对，已经存在则刷新 v
    ret = StrHashSet(StrHashTable, "a", 20);
    ret = StrHashSet(StrHashTable, "b", 10);
    // 查询 k 是否存在
    printf("k=a exist? %d\n", StrHashExist(StrHashTable, "a"));
    // 由 k 查 v
    v = StrHashGet(StrHashTable, "a", &exist);
    printf("k=a exist? %d and v=%d\n", exist, v); 
    // HashTable 长度
    printf("HashTable length %d\n", StrHashSize(StrHashTable));
    // 指定 k 删除 k-v 对
    StrHashDel(StrHashTable, "a");
    StrHashDel(StrHashTable, "b");
    // 查询 k 是否存在
    printf("k=a exist? %d\n", StrHashExist(StrHashTable, "a"));
    /* 数组hash，迭代打印 */
    const char *num[] = {"apple", "apple", "b", "b", "b", "caffee"};
    for (int i = 0; i < sizeof(num) / sizeof(num[0]); i++)
    {
        v = StrHashGet(StrHashTable, num[i], &exist);
        if (exist)
            StrHashSet(StrHashTable, num[i], v + 1);
        else
            StrHashSet(StrHashTable, num[i], 1);
    }
    StrHashIterFunc fun = kvStrPrint;
    StrHashIterate(StrHashTable, fun, NULL);
    // 删除 HashTable
    StrHashDestroy(StrHashTable);
}

int main()
{
    IntHashTest();
    printf("\n");
    StrHashTest();

    system("pause");
    return 0;
}