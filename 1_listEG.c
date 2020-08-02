#include <stdio.h>
#include <stdlib.h>
#include "include/hlist2.h"

void ListPrint(uintptr_t data, void *arg)
{
    printf("%d -> ", data);
}

void ListTest()
{
    uintptr_t node;
    bool exist;
    // 初始化链表
    SimpList *IntList = SimpListCreate();
    // 链表是否为空
    printf("List is empty? %d\n", SimpListEmpty(IntList));
    // 插入结点
    printf("Inset head %d, successful? %d\n", 10, !SimpListAddHead(IntList, 10));
    printf("Inset tail %d, successful? %d\n", 20, !SimpListAddTail(IntList, 20));
    // 获取结点
    node = SimpListHead(IntList, &exist);
    printf("head exist? %d, head=%d\n", exist, node);
    node = SimpListTail(IntList, &exist);
    printf("tail exist? %d, tail=%d\n", exist, node);
    // 链表长度
    printf("List length %d\n", SimpListSize(IntList));
    // 删除节点
    SimpListRemoveHead(IntList);
    SimpListRemoveTail(IntList);
    /* 构建链表，迭代打印 */
    uintptr_t nums[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < sizeof(nums) / sizeof(nums[0]); i++)
    {
        SimpListAddHead(IntList, nums[i]);
    }
    SimpListIterFunc fun = ListPrint;
    SimpListIterate(IntList, fun, NULL);
    // 删除链表
    SimpListDestroy(IntList);
}

int main()
{
    ListTest();
    system("pause");
    return 0;
}