/*
 * Description:  哈希表2，提供哈希表的常用简化接口
 * Create: 2020-06-01
 * Version: V1.4
 */

#ifndef HHASH2_H
#define HHASH2_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "hhash.h"

struct SimpHash;

/* key 为 int, value 为 uintptr_t 的哈希表接口 */
typedef struct SimpHash IntHash;

static inline IntHash *IntHashCreate(void);
static inline void IntHashDestroy(IntHash *ht);

static inline bool IntHashEmpty(const IntHash *ht);
static inline size_t IntHashSize(const IntHash *ht);

/* 增加 k-v 对，已经存在则刷新 v；成功返回 0，内存错误返回 -1 */
static inline int IntHashSet(IntHash *ht, int k, uintptr_t v);
/* 由 k 查 v；出参 exist 可选，指示是否存在 */
static inline uintptr_t IntHashGet(const IntHash *ht, int k, bool *exist);
/* 指定 k 删除 k-v 对 */
static inline void IntHashDel(IntHash *ht, int k);
/* 指定 k 查询是否存在 */
static inline bool IntHashExist(const IntHash *ht, int k);
/* 迭代处理所有节点 k-v */
typedef void (*IntHashIterFunc)(int k, uintptr_t v, void *arg);
static inline void IntHashIterate(const IntHash *ht, IntHashIterFunc fn, void *arg);


/* key 为 str(const char*), value 为 uintptr_t 的哈希表接口 */
typedef struct SimpHash StrHash;

static inline StrHash *StrHashCreate(void);
static inline void StrHashDestroy(StrHash *ht);

static inline bool StrHashEmpty(const StrHash *ht);
static inline size_t StrHashSize(const StrHash *ht);

/* 增加 k-v 对，已经存在则刷新 v；成功返回 0，内存错误返回 -1 */
static inline int StrHashSet(StrHash *ht, const char *k, uintptr_t v);
/* 由 k 查 v；出参 *exist 指示是否存在 */
static inline uintptr_t StrHashGet(const StrHash *ht, const char *k, bool *exist);
/* 指定 k 删除 k-v 对 */
static inline void StrHashDel(StrHash *ht, const char *k);
/* 指定 k 查询是否存在 */
static inline bool StrHashExist(const StrHash *ht, const char *k);
/* 迭代处理所有节点 k-v */
typedef void (*StrHashIterFunc)(const char *k, uintptr_t v, void *arg);
static inline void StrHashIterate(const StrHash *ht, StrHashIterFunc fn, void *arg);


/* IntHash, StrHash 复用相同的节点类型 */
struct SimpHashNode {
    struct Node node;
    int intk;
    const char *strk;
    uintptr_t value;
};

static bool IntNodeEqual(const struct Node *a, const struct Node *b)
{
    struct SimpHashNode *na = NODE_ENTRY(a, struct SimpHashNode, node);
    struct SimpHashNode *nb = NODE_ENTRY(b, struct SimpHashNode, node);
    return na->intk == nb->intk;
}

static bool StrNodeEqual(const struct Node *a, const struct Node *b)
{
    struct SimpHashNode *na = NODE_ENTRY(a, struct SimpHashNode, node);
    struct SimpHashNode *nb = NODE_ENTRY(b, struct SimpHashNode, node);
    return strcmp(na->strk, nb->strk) == 0;
}

static size_t IntNodeKey(const struct Node *node, size_t bktSize)
{
    struct SimpHashNode *n = NODE_ENTRY(node, struct SimpHashNode, node);
    size_t k = TwIntHash((unsigned int)n->intk);
    return k % bktSize;
}

static size_t StrNodeKey(const struct Node *node, size_t bktSize)
{
    struct SimpHashNode *n = NODE_ENTRY(node, struct SimpHashNode, node);
    size_t k = BkdrHash(n->strk);
    return k % bktSize;
}

static void SimpHashNodeFree(struct Node *node)
{
    struct SimpHashNode *n = NODE_ENTRY(node, struct SimpHashNode, node);
    free(n);
}

#define SIMPLE_HASH_INIT_BKT_SIZE 16

/* 
 * 每个 hash 桶中节点个数平均值上限
 * 当节点增多，导致超过这个值时，hash 桶自动扩充一倍
 * 如果有内存空间限制，可以增大该值，对应的搜索时间会增加
 */
#define SIMPLE_HASH_NODE_PER_BKT 1

struct SimpHash {
    struct HashTable raw;
    size_t nodeCnt;
};

static inline IntHash *IntHashCreate(void)
{
    struct SimpHash *ht = (struct SimpHash*)malloc(sizeof(struct SimpHash));
    if (ht == NULL) {
        return NULL;
    }
    ht->nodeCnt = 0;
    int ret = HashInit(&ht->raw, SIMPLE_HASH_INIT_BKT_SIZE, IntNodeEqual, IntNodeKey);
    if (ret != 0) {
        free(ht);
        return NULL;
    }
    return ht;
}

static inline StrHash *StrHashCreate(void)
{
    struct SimpHash *ht = (struct SimpHash*)malloc(sizeof(struct SimpHash));
    if (ht == NULL) {
        return NULL;
    }
    ht->nodeCnt = 0;
    int ret = HashInit(&ht->raw, SIMPLE_HASH_INIT_BKT_SIZE, StrNodeEqual, StrNodeKey);
    if (ret != 0) {
        free(ht);
        return NULL;
    }
    return ht;
}

static inline void IntHashDestroy(IntHash *ht)
{
    HashDeinit(&ht->raw, SimpHashNodeFree);
    free(ht);
}

static inline void StrHashDestroy(StrHash *ht)
{
    HashDeinit(&ht->raw, SimpHashNodeFree);
    free(ht);
}

static inline bool IntHashEmpty(const IntHash *ht)
{
    return ht->nodeCnt == 0;
}

static inline bool StrHashEmpty(const StrHash *ht)
{
    return ht->nodeCnt == 0;
}

static inline size_t IntHashSize(const IntHash *ht)
{
    return ht->nodeCnt;
}

static inline size_t StrHashSize(const StrHash *ht)
{
    return ht->nodeCnt;
}

static inline int IntHashSet(IntHash *ht, int k, uintptr_t v)
{
    struct SimpHashNode cmp = { .intk = k };
    struct SimpHashNode *n;
    struct Node *node = HashFind(&ht->raw, &cmp.node);
    if (node != NULL) {
        n = NODE_ENTRY(node, struct SimpHashNode, node);
        n->value = v;
        return 0;
    }

    n = (struct SimpHashNode*)malloc(sizeof(struct SimpHashNode));
    if (n == NULL) {
        return -1;
    }
    n->intk = k;
    n->value = v;
    HashAdd(&ht->raw, &n->node);
    ht->nodeCnt++;
    if (ht->nodeCnt > ht->raw.bktSize * SIMPLE_HASH_NODE_PER_BKT) {
        (void)HashReset(&ht->raw, ht->raw.bktSize * 2, NULL, NULL);
    }
    return 0;
}

static inline int StrHashSet(StrHash *ht, const char *k, uintptr_t v)
{
    struct SimpHashNode cmp = { .strk = k };
    struct SimpHashNode *n;
    struct Node *node = HashFind(&ht->raw, &cmp.node);
    if (node != NULL) {
        n = NODE_ENTRY(node, struct SimpHashNode, node);
        n->value = v;
        return 0;
    }

    size_t len = sizeof(struct SimpHashNode) + strlen(k) + 1;
    n = (struct SimpHashNode*)malloc(len);
    if (n == NULL) {
        return -1;
    }
    (void)strcpy((char*)(n + 1), k);
    n->strk = (const char*)(n + 1);
    n->value = v;
    HashAdd(&ht->raw, &n->node);
    ht->nodeCnt++;
    if (ht->nodeCnt > ht->raw.bktSize * SIMPLE_HASH_NODE_PER_BKT) {
        (void)HashReset(&ht->raw, ht->raw.bktSize * 2, NULL, NULL);
    }
    return 0;
}

static inline uintptr_t IntHashGet(const IntHash *ht, int k, bool *exist)
{
    struct SimpHashNode cmp = { .intk = k };
    struct Node *node = HashFind(&ht->raw, &cmp.node);
    if (exist != NULL) {
        *exist = node != NULL;
    }
    if (node != NULL) {
        struct SimpHashNode *n = NODE_ENTRY(node, struct SimpHashNode, node);
        return n->value;
    }
    return 0;
}

static inline uintptr_t StrHashGet(const StrHash *ht, const char *k, bool *exist)
{
    struct SimpHashNode cmp = { .strk = k };
    struct Node *node = HashFind(&ht->raw, &cmp.node);
    if (exist != NULL) {
        *exist = node != NULL;
    }
    if (node != NULL) {
        struct SimpHashNode *n = NODE_ENTRY(node, struct SimpHashNode, node);
        return n->value;
    }
    return 0;
}

static inline void IntHashDel(IntHash *ht, int k)
{
    struct SimpHashNode cmp = { .intk = k };
    struct Node *node = HashFind(&ht->raw, &cmp.node);
    if (node == NULL) {
        return;
    }
    HashRemove(node);
    ht->nodeCnt--;
    struct SimpHashNode *n = NODE_ENTRY(node, struct SimpHashNode, node);
    free(n);
}

static inline void StrHashDel(StrHash *ht, const char *k)
{
    struct SimpHashNode cmp = { .strk = k };
    struct Node *node = HashFind(&ht->raw, &cmp.node);
    if (node == NULL) {
        return;
    }
    HashRemove(node);
    ht->nodeCnt--;
    struct SimpHashNode *n = NODE_ENTRY(node, struct SimpHashNode, node);
    free(n);
}

static inline bool IntHashExist(const IntHash *ht, int k)
{
    struct SimpHashNode cmp = { .intk = k };
    struct Node *node = HashFind(&ht->raw, &cmp.node);
    return node != NULL;
}

static inline bool StrHashExist(const StrHash *ht, const char *k)
{
    struct SimpHashNode cmp = { .strk = k };
    struct Node *node = HashFind(&ht->raw, &cmp.node);
    return node != NULL;
}

static inline void IntHashIterate(const IntHash *ht, IntHashIterFunc fn, void *arg)
{
    for (size_t i = 0; i < ht->raw.bktSize; i++) {
        struct Node *node;
        LIST_FOR_EACH(node, &ht->raw.bkts[i]) {
            struct SimpHashNode *n = NODE_ENTRY(node, struct SimpHashNode, node);
            fn(n->intk, n->value, arg);
        }
    }
}

static inline void StrHashIterate(const StrHash *ht, StrHashIterFunc fn, void *arg)
{
    for (size_t i = 0; i < ht->raw.bktSize; i++) {
        struct Node *node;
        LIST_FOR_EACH(node, &ht->raw.bkts[i]) {
            struct SimpHashNode *n = NODE_ENTRY(node, struct SimpHashNode, node);
            fn(n->strk, n->value, arg);
        }
    }
}

#endif /* HHASH2_H */