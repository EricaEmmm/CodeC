/*
 * Description:  链表2，提供常用简化接口
 * Create: 2020-06-01
 * Version: V1.4
 */

#ifndef HLIST2_H
#define HLIST2_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "hlist.h"

/* 提供数据类型为 uintptr_t 的链表简化接口 */
typedef struct {
    struct List raw;
    size_t nodeCnt;
} SimpList;

static inline SimpList *SimpListCreate(void);
static inline void SimpListDestroy(SimpList *list);

static inline bool SimpListEmpty(const SimpList *list);
static inline size_t SimpListSize(const SimpList *list);

/* 获取头节点数据，出参 exist 可选，指示是否存在 */
static inline uintptr_t SimpListHead(const SimpList *list, bool *exist);
/* 获取尾节点数据，出参 exist 可选，指示是否存在 */
static inline uintptr_t SimpListTail(const SimpList *list, bool *exist);
/* 插入新头节点；成功返回 0，内存错误返回 -1 */
static inline int SimpListAddHead(SimpList *list, uintptr_t data);
/* 插入新尾节点；成功返回 0，内存错误返回 -1 */
static inline int SimpListAddTail(SimpList *list, uintptr_t data);
/* 删除头节点 */
static inline void SimpListRemoveHead(SimpList *list);
/* 删除尾节点 */
static inline void SimpListRemoveTail(SimpList *list);

/* 迭代处理所有节点数据，参数 arg 可选 */
typedef void (*SimpListIterFunc)(uintptr_t data, void *arg);
static inline void SimpListIterate(const SimpList *list, SimpListIterFunc fn, void *arg);


/* 通过 pos 进行遍历或操作中间节点 */
typedef struct Node SimpListPos;

/* 获取头节点位置，空表时返回 NULL */
static inline SimpListPos *SimpListHeadPos(SimpList *list);
/* 获取尾节点位置，空表时返回 NULL */
static inline SimpListPos *SimpListTailPos(SimpList *list);
/* 获取下一节点位置，遍历结束返回 NULL */
static inline SimpListPos *SimpListNextPos(SimpList *list, SimpListPos *pos);
/* 获取上一节点位置，遍历结束返回 NULL */
static inline SimpListPos *SimpListPrevPos(SimpList *list, SimpListPos *pos);

/* 获取 pos 处节点值 */
static inline uintptr_t SimpListPosData(SimpListPos *pos);

/* pos 处插入新节点，原节点后移；返回新节点 pos，内存失败时返回 NULL */
static inline SimpListPos *SimpListPosInsert(SimpList *list, SimpListPos *pos, uintptr_t data);
/* 删除 pos 处节点；返回被删除节点的 "下一个" pos，可能为空 */
static inline SimpListPos *SimpListPosRemove(SimpList *list, SimpListPos *pos);


typedef struct {
    struct Node node;
    uintptr_t data;
} SimpListNode;

static void SimpListNodeFree(struct Node *node)
{
    SimpListNode *n = NODE_ENTRY(node, SimpListNode, node);
    free(n);
}

static inline SimpList *SimpListCreate(void)
{
    SimpList *list;
    list = (SimpList*)malloc(sizeof(SimpList));
    if (list == NULL) {
        return NULL;
    }
    ListInit(&list->raw);
    list->nodeCnt = 0;
    return list;
}

static inline void SimpListDestroy(SimpList *list)
{
    ListDeinit(&list->raw, SimpListNodeFree);
    free(list);
}

static inline bool SimpListEmpty(const SimpList *list)
{
    return ListEmpty(&list->raw);
}

static inline size_t SimpListSize(const SimpList *list)
{
    return list->nodeCnt;
}

static inline uintptr_t SimpListHead(const SimpList *list, bool *exist)
{
    SimpListNode *n = LIST_HEAD_ENTRY(&list->raw, SimpListNode, node);
    if (exist != NULL) {
        *exist = n != NULL;
    }
    if (n != NULL) {
        return n->data;
    }
    return 0;
}

static inline uintptr_t SimpListTail(const SimpList *list, bool *exist)
{
    SimpListNode *n = LIST_TAIL_ENTRY(&list->raw, SimpListNode, node);
    if (exist != NULL) {
        *exist = n != NULL;
    }
    if (n != NULL) {
        return n->data;
    }
    return 0;
}

static inline int SimpListAddHead(SimpList *list, uintptr_t data)
{
    SimpListNode *n = (SimpListNode*)malloc(sizeof(SimpListNode));
    if (n == NULL) {
        return -1;
    }
    n->data = data;
    ListAddHead(&list->raw, &n->node);
    list->nodeCnt++;
    return 0;
}

static inline int SimpListAddTail(SimpList *list, uintptr_t data)
{
    SimpListNode *n = (SimpListNode*)malloc(sizeof(SimpListNode));
    if (n == NULL) {
        return -1;
    }
    n->data = data;
    ListAddTail(&list->raw, &n->node);
    list->nodeCnt++;
    return 0;
}

static inline void SimpListRemoveHead(SimpList *list)
{
    SimpListNode *n = LIST_HEAD_ENTRY(&list->raw, SimpListNode, node);
    if (n == NULL) {
        return;
    }
    ListRemove(&n->node);
    list->nodeCnt--;
    free(n);
}

static inline void SimpListRemoveTail(SimpList *list)
{
    SimpListNode *n = LIST_TAIL_ENTRY(&list->raw, SimpListNode, node);
    if (n == NULL) {
        return;
    }
    ListRemove(&n->node);
    list->nodeCnt--;
    free(n);
}

static inline void SimpListIterate(const SimpList *list, SimpListIterFunc fn, void *arg)
{
    struct Node *node;
    LIST_FOR_EACH(node, &list->raw) {
        SimpListNode *n = NODE_ENTRY(node, SimpListNode, node);
        fn(n->data, arg);
    }
}

static inline SimpListPos *SimpListHeadPos(SimpList *list)
{
    if (ListEmpty(&list->raw)) {
        return NULL;
    }
    return list->raw.base.next;
}

static inline SimpListPos *SimpListTailPos(SimpList *list)
{
    if (ListEmpty(&list->raw)) {
        return NULL;
    }
    return list->raw.base.prev;
}

static inline SimpListPos *SimpListNextPos(SimpList *list, SimpListPos *pos)
{
    if (ListIsTail(&list->raw, pos)) {
        return NULL;
    }
    return pos->next;
}

static inline SimpListPos *SimpListPrevPos(SimpList *list, SimpListPos *pos)
{
    if (ListIsHead(&list->raw, pos)) {
        return NULL;
    }
    return pos->prev;
}

static inline uintptr_t SimpListPosData(SimpListPos *pos)
{
    SimpListNode *n = NODE_ENTRY(pos, SimpListNode, node);
    return n->data;
}

static inline SimpListPos *SimpListPosInsert(SimpList *list, SimpListPos *pos, uintptr_t data)
{
    SimpListNode *n = (SimpListNode*)malloc(sizeof(SimpListNode));
    if (n == NULL) {
        return NULL;
    }
    n->data = data;
    ListInsert(pos, &n->node);
    list->nodeCnt++;
    return &n->node;
}

static inline SimpListPos *SimpListPosRemove(SimpList *list, SimpListPos *pos)
{
    SimpListPos *next = SimpListNextPos(list, pos);
    ListRemove(pos);
    list->nodeCnt--;
    return next;
}

#endif /* HLIST2_H */