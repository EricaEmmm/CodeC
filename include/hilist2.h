/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description:  双向链表易用版
 *               数据类型为 uintptr_t
 * Create: 2020-06-01
 * Version: V1.5
 * Contact: 有任何问题或建议，请前往
 *          http://rnd-isource.huawei.com/bugs/hw-cstl
 */

#ifndef HILIST2_H
#define HILIST2_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "hilist.h"

typedef struct {
    struct HiList raw;
    size_t nodeCnt;
} HiSimpList;

static inline HiSimpList *HiSimpListCreate(void);
static inline void HiSimpListDestroy(HiSimpList *list);

static inline bool HiSimpListEmpty(const HiSimpList *list);
static inline size_t HiSimpListSize(const HiSimpList *list);

/* 获取头节点数据，出参 exist 可选，指示是否存在 */
static inline uintptr_t HiSimpListHead(const HiSimpList *list, bool *exist);
/* 获取尾节点数据，出参 exist 可选，指示是否存在 */
static inline uintptr_t HiSimpListTail(const HiSimpList *list, bool *exist);
/* 插入新头节点；成功返回 0，内存错误返回 -1 */
static inline int HiSimpListAddHead(HiSimpList *list, uintptr_t data);
/* 插入新尾节点；成功返回 0，内存错误返回 -1 */
static inline int HiSimpListAddTail(HiSimpList *list, uintptr_t data);
/* 删除头节点 */
static inline void HiSimpListRemoveHead(HiSimpList *list);
/* 删除尾节点 */
static inline void HiSimpListRemoveTail(HiSimpList *list);

/* 迭代处理所有节点数据，参数 arg 可选 */
typedef void (*HiSimpListIterFunc)(uintptr_t data, void *arg);
static inline void HiSimpListWalk(const HiSimpList *list, 
                                  HiSimpListIterFunc fn,
                                  void *arg);


/* 通过 pos 进行遍历或操作中间节点 */
typedef struct HiNode HiSimpListPos;

/* 获取 pos 处节点用户数据 */
static inline uintptr_t HiSimpListPosData(HiSimpListPos *pos);

/* 获取头节点位置，空表时返回 NULL */
static inline HiSimpListPos *HiSimpListHeadPos(HiSimpList *list);
/* 获取尾节点位置，空表时返回 NULL */
static inline HiSimpListPos *HiSimpListTailPos(HiSimpList *list);
/* 获取下一节点位置，遍历结束返回 NULL */
static inline HiSimpListPos *HiSimpListNextPos(HiSimpList *list, HiSimpListPos *pos);
/* 获取上一节点位置，遍历结束返回 NULL */
static inline HiSimpListPos *HiSimpListPrevPos(HiSimpList *list, HiSimpListPos *pos);

/* 
 * 在 pos 处插入新节点，原节点后移；pos 为 NULL 时，插入到链表尾。
 * 返回新节点 pos，内存失败时返回 NULL
 */
static inline HiSimpListPos *HiSimpListPosInsert(HiSimpList *list,
                                                 HiSimpListPos *pos,
                                                 uintptr_t data);
/* 删除 pos 处节点；返回被删除节点的 "下一个" pos，可能为空 */
static inline HiSimpListPos *HiSimpListPosRemove(HiSimpList *list, HiSimpListPos *pos);


typedef struct {
    struct HiNode node;
    uintptr_t data;
} HiSimpListNode;

static void HiSimpListNodeFree(struct HiNode *node)
{
    HiSimpListNode *n = HI_NODE_ENTRY(node, HiSimpListNode, node);
    free(n);
}

static inline HiSimpList *HiSimpListCreate(void)
{
    HiSimpList *list;
    list = (HiSimpList*)malloc(sizeof(HiSimpList));
    if (list == NULL) {
        return NULL;
    }
    HiListInit(&list->raw);
    list->nodeCnt = 0;
    return list;
}

static inline void HiSimpListDestroy(HiSimpList *list)
{
    HiListDeinit(&list->raw, HiSimpListNodeFree);
    free(list);
}

static inline bool HiSimpListEmpty(const HiSimpList *list)
{
    return HiListEmpty(&list->raw);
}

static inline size_t HiSimpListSize(const HiSimpList *list)
{
    return list->nodeCnt;
}

static inline uintptr_t HiSimpListHead(const HiSimpList *list, bool *exist)
{
    HiSimpListNode *n = HI_LIST_HEAD_ENTRY(&list->raw, HiSimpListNode, node);
    if (exist != NULL) {
        *exist = n != NULL;
    }
    if (n != NULL) {
        return n->data;
    }
    return 0;
}

static inline uintptr_t HiSimpListTail(const HiSimpList *list, bool *exist)
{
    HiSimpListNode *n = HI_LIST_TAIL_ENTRY(&list->raw, HiSimpListNode, node);
    if (exist != NULL) {
        *exist = n != NULL;
    }
    if (n != NULL) {
        return n->data;
    }
    return 0;
}

static inline int HiSimpListAddHead(HiSimpList *list, uintptr_t data)
{
    HiSimpListNode *n = (HiSimpListNode*)malloc(sizeof(HiSimpListNode));
    if (n == NULL) {
        return -1;
    }
    n->data = data;
    HiListAddHead(&list->raw, &n->node);
    list->nodeCnt++;
    return 0;
}

static inline int HiSimpListAddTail(HiSimpList *list, uintptr_t data)
{
    HiSimpListNode *n = (HiSimpListNode*)malloc(sizeof(HiSimpListNode));
    if (n == NULL) {
        return -1;
    }
    n->data = data;
    HiListAddTail(&list->raw, &n->node);
    list->nodeCnt++;
    return 0;
}

static inline void HiSimpListRemoveHead(HiSimpList *list)
{
    HiSimpListNode *n = HI_LIST_HEAD_ENTRY(&list->raw, HiSimpListNode, node);
    if (n == NULL) {
        return;
    }
    HiListRemove(&n->node);
    list->nodeCnt--;
    free(n);
}

static inline void HiSimpListRemoveTail(HiSimpList *list)
{
    HiSimpListNode *n = HI_LIST_TAIL_ENTRY(&list->raw, HiSimpListNode, node);
    if (n == NULL) {
        return;
    }
    HiListRemove(&n->node);
    list->nodeCnt--;
    free(n);
}

static inline void HiSimpListWalk(const HiSimpList *list, 
                                  HiSimpListIterFunc fn, 
                                  void *arg)
{
    struct HiNode *node;
    HI_LIST_FOR_EACH(node, &list->raw) {
        HiSimpListNode *n = HI_NODE_ENTRY(node, HiSimpListNode, node);
        fn(n->data, arg);
    }
}

static inline uintptr_t HiSimpListPosData(HiSimpListPos *pos)
{
    HiSimpListNode *n = HI_NODE_ENTRY(pos, HiSimpListNode, node);
    return n->data;
}

static inline HiSimpListPos *HiSimpListHeadPos(HiSimpList *list)
{
    if (HiListEmpty(&list->raw)) {
        return NULL;
    }
    return list->raw.base.next;
}

static inline HiSimpListPos *HiSimpListTailPos(HiSimpList *list)
{
    if (HiListEmpty(&list->raw)) {
        return NULL;
    }
    return list->raw.base.prev;
}

static inline HiSimpListPos *HiSimpListNextPos(HiSimpList *list, HiSimpListPos *pos)
{
    if (HiListIsTail(&list->raw, pos)) {
        return NULL;
    }
    return pos->next;
}

static inline HiSimpListPos *HiSimpListPrevPos(HiSimpList *list, HiSimpListPos *pos)
{
    if (HiListIsHead(&list->raw, pos)) {
        return NULL;
    }
    return pos->prev;
}

static inline HiSimpListPos *HiSimpListPosInsert(HiSimpList *list, 
                                                 HiSimpListPos *pos, 
                                                 uintptr_t data)
{
    HiSimpListNode *n = (HiSimpListNode*)malloc(sizeof(HiSimpListNode));
    if (n == NULL) {
        return NULL;
    }
    n->data = data;
    if (pos == NULL) {
        pos = &list->raw.base;
    }
    HiListInsert(pos, &n->node);
    list->nodeCnt++;
    return &n->node;
}

static inline HiSimpListPos *HiSimpListPosRemove(HiSimpList *list, HiSimpListPos *pos)
{
    HiSimpListPos *next = HiSimpListNextPos(list, pos);
    HiListRemove(pos);
    HiSimpListNode *n = HI_NODE_ENTRY(pos, HiSimpListNode, node);
    free(n);
    list->nodeCnt--;
    return next;
}

#endif /* HILIST2_H */