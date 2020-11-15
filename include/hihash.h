/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: 哈希表
 * Create: 2019-11-11
 * Version: V1.5
 * Contact: 有任何问题或建议，请前往
 *          http://rnd-isource.huawei.com/bugs/hw-cstl
 */

#ifndef HIHASH_H
#define HIHASH_H

#include <stdbool.h>
#include <stdlib.h>

/* 基于链表，链地址法 */
#include "hilist.h"

/* 
 * 用户定义，判断两个节点是否相同
 * 入参是 HiNode 指针！可使用 NODE_ENTRY 获取节点数据
 */
typedef bool (*HiHashEqualFunc)(const struct HiNode *a, const struct HiNode *b);

/*
 * 用户定义，hash key 生成函数，用于索引 hash 桶
 * 返回值不应超过 hash 桶大小限制！
 * 入参是 HiNode 指针！可使用 NODE_ENTRY 获取节点数据
 */
typedef size_t (*HiHashKeyFunc)(const struct HiNode *node, size_t bktSize);

/* 
 * Thomas Wong 整数散列函数
 * 你可以按需使用，并实现你的 HiHashKeyFunc
 * 注意：返回值或超过哈希桶大小，需要进一步处理！
 */
static inline size_t HiTwIntHash(unsigned int key)
{
    key = ~key + (key << 15);
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = (key + (key << 3)) + (key << 11);
    key = key ^ (key >> 16);
    return (size_t)key;
}

/* 
 * BKDR 字符串散列函数
 * 你可以按需使用，并实现你的 HiHashKeyFunc
 * 注意：返回值或超过哈希桶大小，需要进一步处理！
 */
static inline size_t HiBkdrHash(const char *str)
{
    size_t hash = 0;
    register size_t ch = 0;
    while ((ch = (size_t)(*str++)) != '\0') {
        // hash = hash * 131 + ch;
        hash = (hash << 7) + (hash << 1) + hash + ch;
    }
    return hash;
}

struct HiHashTable {
    size_t bktSize;
    HiHashEqualFunc equal;
    HiHashKeyFunc key;
    struct HiList *bkts;
};

/* 成功返回 0；失败返回 -1 */
static inline int HiHashInit(struct HiHashTable *ht, 
                             size_t bktSize, 
                             HiHashEqualFunc equal, 
                             HiHashKeyFunc key)
{
    ht->bktSize = bktSize;
    ht->equal = equal;
    ht->key = key;
    ht->bkts = (struct HiList*)malloc(sizeof(struct HiList) * bktSize);
    if (ht->bkts == NULL) {
        return -1;
    }

    size_t i;
    for (i = 0; i < bktSize; i++) {
        HiListInit(&ht->bkts[i]);
    }
    return 0;
}

/* 注意：nodeDeinit 函数入参是 HiNode 指针! */
static inline void HiHashDeinit(struct HiHashTable *ht, HiNodeFunc nodeDeinit)
{
    if (nodeDeinit != NULL) {
        size_t i;
        for (i = 0; i < ht->bktSize; i++) {
            HiListDeinit(&ht->bkts[i], nodeDeinit);
        }
    }
    free(ht->bkts);
}

static inline void HiHashAdd(struct HiHashTable *ht, struct HiNode *node)
{
    size_t k = ht->key(node, ht->bktSize);
    struct HiList *list = &ht->bkts[k];
    HiListAddTail(list, node);
}

static inline void HiHashRemove(struct HiNode *node)
{
    HiListRemove(node);
}

static inline struct HiNode *HiHashFind(const struct HiHashTable *ht, 
                                        const struct HiNode *cmpNode)
{
    size_t k = ht->key(cmpNode, ht->bktSize);
    struct HiList *list = &ht->bkts[k];
    struct HiNode *node;
    HI_LIST_FOR_EACH(node, list) {
        if (ht->equal(cmpNode, node)) {
            return node;
        }
    }
    return NULL;
}

/* 
 * 遍历处理哈希节点
 * 注意: nodeProc 函数入参是 HiNode 指针！
 *       遍历时不要改变 key 值及链表结构！
 */
static inline void HiHashWalk(struct HiHashTable *ht,
                              HiNodeFuncX nodeProc,
                              void *arg)
{
    size_t i;
    for (i = 0; i < ht->bktSize; i++) {
        struct HiNode *node;
        HI_LIST_FOR_EACH(node, &ht->bkts[i]) {
            nodeProc(node, arg);
        }
    }
}

/* 
 * 重置哈希表参数并触发 rehash
 * 传入零值参数可保持原值不变
 * 成功返回 0；失败返回 -1；失败时，原哈希表仍可使用
 */
static inline int HiHashReset(struct HiHashTable *ht, 
                              size_t bktSize, 
                              HiHashEqualFunc equal, 
                              HiHashKeyFunc key)
{
    bktSize = bktSize != 0 ? bktSize : ht->bktSize;
    equal = equal != NULL ? equal : ht->equal;
    key = key != NULL ? key : ht->key;

    struct HiHashTable newHt;
    int ret = HiHashInit(&newHt, bktSize, equal, key);
    if (ret != 0) {
        return -1;
    }

    size_t i;
    for (i = 0; i < ht->bktSize; i++) {
        struct HiList *list = &ht->bkts[i];
        struct HiNode *node, *tmp;
        HI_LIST_FOR_EACH_SAFE(node, tmp, list) {
            HiHashAdd(&newHt, node);
        }
    }

    free(ht->bkts);
    *ht = newHt;
    return 0;
}

#endif /* HIHASH_H */