/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description:  哈希表易用版
 *               键值类型为 int 或 str(const char*)；数据类型为 uintptr_t
 * Create: 2020-06-01
 * Version: V1.5
 * Contact: 有任何问题或建议，请前往
 *          http://rnd-isource.huawei.com/bugs/hw-cstl
 */

#ifndef HIHASH2_H
#define HIHASH2_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "hihash.h"

struct HiSimpHash;

/* key 为 int, value 为 uintptr_t 的哈希表接口 */
typedef struct HiSimpHash HiIntHash;

static inline HiIntHash *HiIntHashCreate(void);
static inline void HiIntHashDestroy(HiIntHash *ht);

static inline bool HiIntHashEmpty(const HiIntHash *ht);
static inline size_t HiIntHashSize(const HiIntHash *ht);

/* 增加 k-v 对，已经存在则刷新 v；成功返回 0，内存错误返回 -1 */
static inline int HiIntHashPut(HiIntHash *ht, int k, uintptr_t v);
/* 由 k 查 v；出参 exist 可选，指示是否存在 */
static inline uintptr_t HiIntHashGet(const HiIntHash *ht, int k, bool *exist);
/* 指定 k 删除 k-v 对 */
static inline void HiIntHashDel(HiIntHash *ht, int k);
/* 指定 k 查询是否存在 */
static inline bool HiIntHashExist(const HiIntHash *ht, int k);
/* 迭代处理所有节点 k-v, 参数 arg 可选 */
typedef void (*HiIntHashWalkFunc)(int k, uintptr_t v, void *arg);
static inline void HiIntHashWalk(const HiIntHash *ht, HiIntHashWalkFunc fn, void *arg);


/* key 为 str(const char*), value 为 uintptr_t 的哈希表接口 */
typedef struct HiSimpHash HiStrHash;

static inline HiStrHash *HiStrHashCreate(void);
static inline void HiStrHashDestroy(HiStrHash *ht);

static inline bool HiStrHashEmpty(const HiStrHash *ht);
static inline size_t HiStrHashSize(const HiStrHash *ht);

/* 增加 k-v 对，已经存在则刷新 v；成功返回 0，内存错误返回 -1 */
static inline int HiStrHashPut(HiStrHash *ht, const char *k, uintptr_t v);
/* 由 k 查 v；出参 *exist 指示是否存在 */
static inline uintptr_t HiStrHashGet(const HiStrHash *ht, const char *k, bool *exist);
/* 指定 k 删除 k-v 对 */
static inline void HiStrHashDel(HiStrHash *ht, const char *k);
/* 指定 k 查询是否存在 */
static inline bool HiStrHashExist(const HiStrHash *ht, const char *k);
/* 迭代处理所有节点 k-v, 参数 arg 可选 */
typedef void HiStrHashWalkFunc(const char *k, uintptr_t v, void *arg);
static inline void HiStrHashWalk(const HiStrHash *ht, HiStrHashWalkFunc fn, void *arg);


/* HiIntHash, HiStrHash 复用相同的节点类型 */
struct HiSimpHashNode {
    struct HiNode node;
    int intk;
    const char *strk;
    uintptr_t value;
};

static bool IntNodeEqual(const struct HiNode *a, const struct HiNode *b)
{
    struct HiSimpHashNode *na = HI_NODE_ENTRY(a, struct HiSimpHashNode, node);
    struct HiSimpHashNode *nb = HI_NODE_ENTRY(b, struct HiSimpHashNode, node);
    return na->intk == nb->intk;
}

static bool StrNodeEqual(const struct HiNode *a, const struct HiNode *b)
{
    struct HiSimpHashNode *na = HI_NODE_ENTRY(a, struct HiSimpHashNode, node);
    struct HiSimpHashNode *nb = HI_NODE_ENTRY(b, struct HiSimpHashNode, node);
    return strcmp(na->strk, nb->strk) == 0;
}

static size_t IntNodeKey(const struct HiNode *node, size_t bktSize)
{
    struct HiSimpHashNode *n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
    size_t k = HiTwIntHash((unsigned int)n->intk);
    return k % bktSize;
}

static size_t StrNodeKey(const struct HiNode *node, size_t bktSize)
{
    struct HiSimpHashNode *n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
    size_t k = HiBkdrHash(n->strk);
    return k % bktSize;
}

static void HiSimpHashNodeFree(struct HiNode *node)
{
    struct HiSimpHashNode *n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
    free(n);
}

#define SIMPLE_HASH_INIT_BKT_SIZE 16

/* 
 * 每个 hash 桶中节点个数平均值上限
 * 当节点增多，导致超过这个值时，hash 桶自动扩充一倍
 * 如果有内存空间限制，可以增大该值，对应的搜索时间会增加
 */
#define SIMPLE_HASH_NODE_PER_BKT 1

struct HiSimpHash {
    struct HiHashTable raw;
    size_t nodeCnt;
};

static inline HiIntHash *HiIntHashCreate(void)
{
    struct HiSimpHash *ht = (struct HiSimpHash*)malloc(sizeof(struct HiSimpHash));
    if (ht == NULL) {
        return NULL;
    }
    ht->nodeCnt = 0;
    int ret = HiHashInit(&ht->raw, SIMPLE_HASH_INIT_BKT_SIZE, IntNodeEqual, IntNodeKey);
    if (ret != 0) {
        free(ht);
        return NULL;
    }
    return ht;
}

static inline HiStrHash *HiStrHashCreate(void)
{
    struct HiSimpHash *ht = (struct HiSimpHash*)malloc(sizeof(struct HiSimpHash));
    if (ht == NULL) {
        return NULL;
    }
    ht->nodeCnt = 0;
    int ret = HiHashInit(&ht->raw, SIMPLE_HASH_INIT_BKT_SIZE, StrNodeEqual, StrNodeKey);
    if (ret != 0) {
        free(ht);
        return NULL;
    }
    return ht;
}

static inline void HiIntHashDestroy(HiIntHash *ht)
{
    HiHashDeinit(&ht->raw, HiSimpHashNodeFree);
    free(ht);
}

static inline void HiStrHashDestroy(HiStrHash *ht)
{
    HiHashDeinit(&ht->raw, HiSimpHashNodeFree);
    free(ht);
}

static inline bool HiIntHashEmpty(const HiIntHash *ht)
{
    return ht->nodeCnt == 0;
}

static inline bool HiStrHashEmpty(const HiStrHash *ht)
{
    return ht->nodeCnt == 0;
}

static inline size_t HiIntHashSize(const HiIntHash *ht)
{
    return ht->nodeCnt;
}

static inline size_t HiStrHashSize(const HiStrHash *ht)
{
    return ht->nodeCnt;
}

static inline int HiIntHashPut(HiIntHash *ht, int k, uintptr_t v)
{
    struct HiSimpHashNode cmp = { .intk = k };
    struct HiSimpHashNode *n;
    struct HiNode *node = HiHashFind(&ht->raw, &cmp.node);
    if (node != NULL) {
        n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
        n->value = v;
        return 0;
    }

    n = (struct HiSimpHashNode*)malloc(sizeof(struct HiSimpHashNode));
    if (n == NULL) {
        return -1;
    }
    n->intk = k;
    n->value = v;
    HiHashAdd(&ht->raw, &n->node);
    ht->nodeCnt++;
    if (ht->nodeCnt > ht->raw.bktSize * SIMPLE_HASH_NODE_PER_BKT) {
        (void)HiHashReset(&ht->raw, ht->raw.bktSize * 2, NULL, NULL);
    }
    return 0;
}

static inline int HiStrHashPut(HiStrHash *ht, const char *k, uintptr_t v)
{
    struct HiSimpHashNode cmp = { .strk = k };
    struct HiSimpHashNode *n;
    struct HiNode *node = HiHashFind(&ht->raw, &cmp.node);
    if (node != NULL) {
        n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
        n->value = v;
        return 0;
    }

    size_t len = sizeof(struct HiSimpHashNode) + strlen(k) + 1;
    n = (struct HiSimpHashNode*)malloc(len);
    if (n == NULL) {
        return -1;
    }
    (void)strcpy((char*)(n + 1), k);
    n->strk = (const char*)(n + 1);
    n->value = v;
    HiHashAdd(&ht->raw, &n->node);
    ht->nodeCnt++;
    if (ht->nodeCnt > ht->raw.bktSize * SIMPLE_HASH_NODE_PER_BKT) {
        (void)HiHashReset(&ht->raw, ht->raw.bktSize * 2, NULL, NULL);
    }
    return 0;
}

static inline uintptr_t HiIntHashGet(const HiIntHash *ht, int k, bool *exist)
{
    struct HiSimpHashNode cmp = { .intk = k };
    struct HiNode *node = HiHashFind(&ht->raw, &cmp.node);
    if (exist != NULL) {
        *exist = node != NULL;
    }
    if (node != NULL) {
        struct HiSimpHashNode *n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
        return n->value;
    }
    return 0;
}

static inline uintptr_t HiStrHashGet(const HiStrHash *ht, const char *k, bool *exist)
{
    struct HiSimpHashNode cmp = { .strk = k };
    struct HiNode *node = HiHashFind(&ht->raw, &cmp.node);
    if (exist != NULL) {
        *exist = node != NULL;
    }
    if (node != NULL) {
        struct HiSimpHashNode *n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
        return n->value;
    }
    return 0;
}

static inline void HiIntHashDel(HiIntHash *ht, int k)
{
    struct HiSimpHashNode cmp = { .intk = k };
    struct HiNode *node = HiHashFind(&ht->raw, &cmp.node);
    if (node == NULL) {
        return;
    }
    HiHashRemove(node);
    ht->nodeCnt--;
    struct HiSimpHashNode *n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
    free(n);
}

static inline void HiStrHashDel(HiStrHash *ht, const char *k)
{
    struct HiSimpHashNode cmp = { .strk = k };
    struct HiNode *node = HiHashFind(&ht->raw, &cmp.node);
    if (node == NULL) {
        return;
    }
    HiHashRemove(node);
    ht->nodeCnt--;
    struct HiSimpHashNode *n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
    free(n);
}

static inline bool HiIntHashExist(const HiIntHash *ht, int k)
{
    struct HiSimpHashNode cmp = { .intk = k };
    struct HiNode *node = HiHashFind(&ht->raw, &cmp.node);
    return node != NULL;
}

static inline bool HiStrHashExist(const HiStrHash *ht, const char *k)
{
    struct HiSimpHashNode cmp = { .strk = k };
    struct HiNode *node = HiHashFind(&ht->raw, &cmp.node);
    return node != NULL;
}

static inline void HiIntHashWalk(const HiIntHash *ht, HiIntHashWalkFunc fn, void *arg)
{
    for (size_t i = 0; i < ht->raw.bktSize; i++) {
        struct HiNode *node;
        HI_LIST_FOR_EACH(node, &ht->raw.bkts[i]) {
            struct HiSimpHashNode *n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
            fn(n->intk, n->value, arg);
        }
    }
}

static inline void HiStrHashWalk(const HiStrHash *ht, HiStrHashWalkFunc fn, void *arg)
{
    for (size_t i = 0; i < ht->raw.bktSize; i++) {
        struct HiNode *node;
        HI_LIST_FOR_EACH(node, &ht->raw.bkts[i]) {
            struct HiSimpHashNode *n = HI_NODE_ENTRY(node, struct HiSimpHashNode, node);
            fn(n->strk, n->value, arg);
        }
    }
}

#endif /* HIHASH2_H */