#include <stdio.h>
#include <stdlib.h>
#include "include/hilist2.h"

void ListPrint(uintptr_t data, void *arg)
{
    printf("%d -> ", data);
}

void ListTest()
{
    uintptr_t node;
    bool exist;
    
    // 初始化链表
    HiSimpList *IntList = HiSimpListCreate();
    // 链表是否为空
    printf("List is empty? %d\n", HiSimpListEmpty(IntList));
    // 插入结点
    printf("Inset head %d, successful? %d\n", 10, !HiSimpListAddHead(IntList, 10));
    printf("Inset tail %d, successful? %d\n", 20, !HiSimpListAddTail(IntList, 20));
    // 获取结点
    node = HiSimpListHead(IntList, &exist);
    printf("head exist? %d, head=%d\n", exist, node);
    node = HiSimpListTail(IntList, &exist);
    printf("tail exist? %d, tail=%d\n", exist, node);
    // 链表长度
    printf("List length %d\n", HiSimpListSize(IntList));
    // 删除节点
    HiSimpListRemoveHead(IntList);
    HiSimpListRemoveTail(IntList);
    /* 构建链表，迭代打印 */
    uintptr_t nums[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < sizeof(nums) / sizeof(nums[0]); i++)
    {
        HiSimpListAddHead(IntList, nums[i]);
    }
    HiSimpListIterFunc fun = ListPrint;
    HiSimpListWalk(IntList, fun, NULL);
    // 通过 pos 进行遍历或操作中间节点
    HiSimpListPos *pos = HiSimpListHeadPos(IntList);        // 获取头结点位置
    pos = HiSimpListPosInsert(IntList, pos, 100);           // pos节点处插入新节点，原节点后移 100 5 4 3 2 1
    printf("pos num 100? %d\n", HiSimpListPosData(pos));    // 获取 pos 处节点数据
    pos = HiSimpListNextPos(IntList, pos);                  // 获取下一节点位置
    pos = HiSimpListPosRemove(IntList, pos);                // 删除 pos 处节点；返回被删除节点的下一个 100 4 3 2 1
    printf("pos num 4? %d\n", HiSimpListPosData(pos));
    pos = HiSimpListPrevPos(IntList, pos);                  // 获取上一节点位置
    printf("pos num 100? %d\n", HiSimpListPosData(pos));
    // 删除链表
    HiSimpListDestroy(IntList);
}

int main()
{
    ListTest();
    system("pause");
    return 0;
}