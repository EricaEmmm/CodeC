/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: 双向链表
 * Create: 2019-11-11
 * Version: V1.5
 * Contact: 有任何问题或建议，请前往
 *          http://rnd-isource.huawei.com/bugs/hw-cstl
 */

#ifndef HILIST_H
#define HILIST_H

#include <stddef.h>
#include <stdbool.h>

/* 
 * 该结构体用于嵌入到业务数据结构体中，用于实现链表
 * 例：
 *     struct Entry {           // 你的业务数据结构体
 *         ...
 *         struct HiNode node;  // 嵌入其中，位置任意
 *         ...
 *     };
 */
struct HiNode {
    struct HiNode *next, *prev;
};

/* 
 * 由成员地址获取结构体地址
 * 例：
 *     struct Entry entry;
 *     struct HiNode *n = &entry.node;
 *     struct Entry *p = HI_NODE_ENTRY(n, struct Entry, node);
 *     此时 p 指向 entry
 */
#define HI_NODE_ENTRY(node, type, member) \
    ((type*)((char*)(node) - offsetof(type, member)))

/* 
 * 用户定义，针对 node 节点的处理函数
 * 注意: 入参是 HiNode 指针！
 * 你可能需要使用 HI_NODE_ENTRY 来获取并处理你的数据
 */
typedef void (*HiNodeFunc)(struct HiNode *node);

/* 
 * 用户定义，针对 node 节点的处理函数，带可选参数
 * 注意: 入参是 HiNode 指针！
 * 你可能需要使用 HI_NODE_ENTRY 来获取并处理你的数据
 */
typedef void (*HiNodeFuncX)(struct HiNode *node, void *arg);


/* 带哨兵节点的双向链表 */
struct HiList {
    struct HiNode base;
};

static inline void HiListInit(struct HiList *list)
{
    list->base.next = &list->base;
    list->base.prev = &list->base;
}

static inline bool HiListEmpty(const struct HiList *list)
{
    return list->base.next == &list->base;
}

static inline bool HiListIsHead(const struct HiList *list, const struct HiNode *node)
{
    return list->base.next == node;
}

static inline bool HiListIsTail(const struct HiList *list, const struct HiNode *node)
{
    return list->base.prev == node;
}

/* node 插入到 pos 前面 */
static inline void HiListInsert(struct HiNode *pos, struct HiNode *node)
{
    node->prev = pos->prev;
    node->next = pos;
    node->prev->next = node;
    node->next->prev = node;
}

static inline void HiListAddTail(struct HiList *list, struct HiNode *node)
{
    HiListInsert(&list->base, node);
}

static inline void HiListAddHead(struct HiList *list, struct HiNode *node)
{
    HiListInsert(list->base.next, node);
}

static inline void HiListRemove(struct HiNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static inline void HiListRemoveTail(struct HiList *list)
{
    HiListRemove(list->base.prev);
}

static inline void HiListRemoveHead(struct HiList *list)
{
    HiListRemove(list->base.next);
}

static inline void HiListReplace(struct HiNode *old, struct HiNode *node)
{
    node->next = old->next;
    node->next->prev = node;
    node->prev = old->prev;
    node->prev->next = node;
}

#define HI_LIST_FOR_EACH(node, list) \
    for (node = (list)->base.next; \
         node != &(list)->base; \
         node = (node)->next)

#define HI_LIST_FOR_EACH_SAFE(node, tmp, list) \
    for (node = (list)->base.next, tmp = (node)->next; \
         node != &(list)->base; \
         node = tmp, tmp = (node)->next)

/* 
 * 遍历处理链表节点
 * 注意: nodeProc 函数入参是 HiNode 指针！
 *       遍历时不要改变链表结构！
 */
static inline void HiListWalk(struct HiList *list,
                              HiNodeFuncX nodeProc,
                              void *arg)
{
    struct HiNode *node;
    HI_LIST_FOR_EACH(node, list) {
        nodeProc(node, arg);
    }
}

/* 注意：HiNodeFunc 函数入参是 HiNode 指针! */
static inline void HiListDeinit(struct HiList *list, HiNodeFunc nodeDeinit)
{
    if (nodeDeinit == NULL) {
        return;
    }

    struct HiNode *node, *tmp;
    HI_LIST_FOR_EACH_SAFE(node, tmp, list) {
        nodeDeinit(node);
    }
}

/* 获取头结点，或空 */
#define HI_LIST_HEAD_ENTRY(list, type, member) \
    (HiListEmpty(list) ? NULL : HI_NODE_ENTRY((list)->base.next, type, member))

/* 获取尾结点，或空 */
#define HI_LIST_TAIL_ENTRY(list, type, member) \
    (HiListEmpty(list) ? NULL : HI_NODE_ENTRY((list)->base.prev, type, member))

/* 获取下一结点，或空 */
#define HI_LIST_NEXT_ENTRY(entry, list, type, member) \
    (HiListIsTail(list, &(entry)->member) ? \
        NULL : \
        HI_NODE_ENTRY((entry)->member.next, type, member))

/* 获取上一结点，或空 */
#define HI_LIST_PREV_ENTRY(entry, list, type, member) \
    (HiListIsHead(list, &(entry)->member) ? \
        NULL : \
        HI_NODE_ENTRY((entry)->member.prev, type, member))

/* 遍历链表；过程中如需操作链表，请使用 _SAFE 版本 */
#define HI_LIST_FOR_EACH_ENTRY(entry, list, type, member) \
    for (entry = HI_NODE_ENTRY((list)->base.next, type, member); \
         &(entry)->member != &(list)->base; \
         entry = HI_NODE_ENTRY((entry)->member.next, type, member))

#define HI_LIST_FOR_EACH_ENTRY_SAFE(entry, tmp, list, type, member) \
    for (entry = HI_NODE_ENTRY((list)->base.next, type, member), \
         tmp = HI_NODE_ENTRY((entry)->member.next, type, member); \
         &(entry)->member != &(list)->base; \
         entry = tmp, tmp = HI_NODE_ENTRY((entry)->member.next, type, member))

/* 倒序遍历链表；过程中如需操作链表，请使用 _SAFE 版本 */
#define HI_LIST_FOR_EACH_ENTRY_REVERSE(entry, list, type, member) \
    for (entry = HI_NODE_ENTRY((list)->base.prev, type, member); \
         &(entry)->member != &(list)->base; \
         entry = HI_NODE_ENTRY((entry)->member.prev, type, member))

#define HI_LIST_FOR_EACH_ENTRY_REVERSE_SAFE(entry, tmp, list, type, member) \
    for (entry = HI_NODE_ENTRY((list)->base.prev, type, member), \
         tmp = HI_NODE_ENTRY((entry)->member.prev, type, member); \
         &(entry)->member != &(list)->base; \
         entry = tmp, tmp = HI_NODE_ENTRY((entry)->member.prev, type, member))

#endif /* HILIST_H */