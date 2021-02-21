/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: 实现简单 UT
 * Create: 2020-09-26
 * Contact: http://rnd-isource.huawei.com/bugs/hw-cstl
 */

#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>

#define TEST_FUNC_BEGIN() \
    int errCnt_ = 0

#define SHOULD(cond) \
    if (!(cond)) { \
        errCnt_++; \
        (void)printf("Failed at line %d of file %s\n", \
                     __LINE__, __FILE__); \
    }

#define SHOULDNT(cond) SHOULD(!(cond))

#define TEST_FUNC_END() do { \
    (void)printf("%-32s: ", __FUNCTION__); \
    if (errCnt_ == 0) { \
        (void)printf("OK.\n"); \
    } else { \
        (void)printf("%d error(s).\n", errCnt_); \
    } \
} while (0)

#define ASSERT(cond) \
    if (!(cond)) { \
        (void)printf("Assert at line %d of file %s\n", \
                     __LINE__, __FILE__); \
        exit(-1); \
    }

#endif /* TEST_H */