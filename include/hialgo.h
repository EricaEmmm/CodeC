/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: 算法扩展
 * Create: 2020-09-26
 * Version: V1.5.1
 * Contact: 有任何问题或建议，请前往
 *          http://rnd-isource.huawei.com/bugs/hw-cstl
 */

#ifndef HIALGO_H
#define HIALGO_H

#include <stddef.h>

/* 
 * bsearch 扩展
 * 增加出参 left/right，返回命中或不命中时，左右相邻节点
 */
static inline void *HiBsearch(const void *key, const void *base,
                              size_t num, size_t size,
                              int (*cmp)(const void*, const void*),
                              void **left, void **right)
{
    if (num == 0) {
        *left = NULL;
        *right = NULL;
        return NULL;
    }

    if (num == 1) {
        int rc = cmp(key, base);
        if (rc < 0) {
            *left = NULL;
            *right = (void*)base;
            return NULL;
        } else if (rc > 0) {
            *left = (void*)base;
            *right = NULL;
            return NULL;
        } else { // rc == 0
            *left = NULL;
            *right = NULL;
            return (void*)base;
        }
    }

    // num >= 2
    void *mid = (void*)((size_t)base + size * (num / 2));
    int rc = cmp(key, mid);
    if (rc < 0) {
        void *ret = HiBsearch(key, base, num / 2, size, cmp, left, right);
        if (*right == NULL) {
            *right = (void*)mid;
        }
        return ret;
    } else if (rc > 0) {
        void *ret = HiBsearch(key, mid, num - num / 2, size, cmp, left, right);
        if (*left == NULL) {
            *left = (void*)((size_t)mid - size);
        }
        return ret;
    } else { // rc == 0
        *left = mid - size;
        *right = num == 2 ? NULL : (void*)((size_t)mid + size);
        return mid;
    }
}

#endif /* HIALGO_H */