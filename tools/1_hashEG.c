#include <stdio.h>
#include <stdlib.h>
#include "include/hihash2.h"

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
    HiIntHash *HiIntHashTable = HiIntHashCreate();
    // 判断 HashTable 是否为空
    printf("HashTable is enmty? %d\n", HiIntHashEmpty(HiIntHashTable));
    // 增加 k-v 对，已经存在则刷新 v
    ret = HiIntHashPut(HiIntHashTable, 1, 20);
    ret = HiIntHashPut(HiIntHashTable, 2, 10);
    // 查询 k 是否存在
    printf("k=1 exist? %d\n", HiIntHashExist(HiIntHashTable, 1));
    // 由 k 查 v
    v = HiIntHashGet(HiIntHashTable, 1, &exist);
    printf("k=1 exist? %d and v=%d\n", exist, v); 
    // HashTable 长度
    printf("HashTable length %d\n", HiIntHashSize(HiIntHashTable));
    // 指定 k 删除 k-v 对
    HiIntHashDel(HiIntHashTable, 1);
    HiIntHashDel(HiIntHashTable, 2);
    // 查询 k 是否存在
    printf("k=1 exist? %d\n", HiIntHashExist(HiIntHashTable, 1));
    /* 数组hash，迭代打印 */
    int num[] = {1, 1, 1, 1, 2, 2, -2, -3};
    for (int i = 0; i < sizeof(num) / sizeof(num[0]); i++)
    {
        v = HiIntHashGet(HiIntHashTable, num[i], &exist);
        if (exist)
            HiIntHashPut(HiIntHashTable, num[i], v + 1);
        else
            HiIntHashPut(HiIntHashTable, num[i], 1);
    }
    HiIntHashWalkFunc fun = kvIntPrint;
    HiIntHashWalk(HiIntHashTable, fun, NULL);   // HiIntHashWalk(HiIntHashTable, kvIntPrint, NULL);
    // 删除 HashTable
    HiIntHashDestroy(HiIntHashTable);
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
    HiIntHash *HiStrHashTable = HiStrHashCreate();
    // 判断 HashTable 是否为空
    printf("HashTable is enmty? %d\n", HiStrHashEmpty(HiStrHashTable));
    // 增加 k-v 对，已经存在则刷新 v
    ret = HiStrHashPut(HiStrHashTable, "a", 20);
    ret = HiStrHashPut(HiStrHashTable, "b", 10);
    // 查询 k 是否存在
    printf("k=a exist? %d\n", HiStrHashExist(HiStrHashTable, "a"));
    // 由 k 查 v
    v = HiStrHashGet(HiStrHashTable, "a", &exist);
    printf("k=a exist? %d and v=%d\n", exist, v); 
    // HashTable 长度
    printf("HashTable length %d\n", HiStrHashSize(HiStrHashTable));
    // 指定 k 删除 k-v 对
    HiStrHashDel(HiStrHashTable, "a");
    HiStrHashDel(HiStrHashTable, "b");
    // 查询 k 是否存在
    printf("k=a exist? %d\n", HiStrHashExist(HiStrHashTable, "a"));
    /* 数组hash，迭代打印 */
    const char *num[] = {"apple", "apple", "b", "b", "b", "caffee"};
    for (int i = 0; i < sizeof(num) / sizeof(num[0]); i++)
    {
        v = HiStrHashGet(HiStrHashTable, num[i], &exist);
        if (exist)
            HiStrHashPut(HiStrHashTable, num[i], v + 1);
        else
            HiStrHashPut(HiStrHashTable, num[i], 1);
    }
    //HiStrHashWalkFunc fun = kvStrPrint;
    HiStrHashWalk(HiStrHashTable, kvStrPrint, NULL);
    // 删除 HashTable
    HiStrHashDestroy(HiStrHashTable);
}

int main()
{
    IntHashTest();
    printf("\n");
    StrHashTest();

    system("pause");
    return 0;
}