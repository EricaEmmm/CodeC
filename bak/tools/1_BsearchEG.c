#include <stdio.h>
#include <stdlib.h>
#include "include/hialgo.h"
#include "include/test.h"

int IntCmp(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

void BsearchTest(void) {
    TEST_FUNC_BEGIN();

    int a[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    int num = sizeof(a) / sizeof(a[0]);
    int size = sizeof(int);

    int c;
    void *ret, *left, *right;

    c = 5;
    ret = HiBsearch(&c, a, num - 1, size, IntCmp, &left, &right);
    SHOULD(left == NULL);
    SHOULD(ret == NULL);
    SHOULD(right = &a[0]);

    c = 10;
    ret = HiBsearch(&c, a, num - 1, size, IntCmp, &left, &right);
    SHOULD(left == NULL);
    SHOULD(ret == &a[0]);
    SHOULD(right = &a[1]);

    c = 11;
    ret = HiBsearch(&c, a, num - 1, size, IntCmp, &left, &right);
    SHOULD(left == &a[0]);
    SHOULD(ret == NULL);
    SHOULD(right = &a[1]);

    c = 99;
    ret = HiBsearch(&c, a, num, size, IntCmp, &left, &right);
    SHOULD(*(int*)left == 90);
    SHOULD(ret == NULL);
    SHOULD(*(int*)right == 100);

    c = 100;
    ret = HiBsearch(&c, a, num, size, IntCmp, &left, &right);
    SHOULD(*(int*)left == 90);
    SHOULD(*(int*)ret == 100);
    SHOULD(right == NULL);

    c = 101;
    ret = HiBsearch(&c, a, num, size, IntCmp, &left, &right);
    SHOULD(*(int*)left == 100);
    SHOULD(ret == NULL);
    SHOULD(right == NULL);

    TEST_FUNC_END();
}

int main(void)
{
    BsearchTest();

    system("pause");
    return 0;
}