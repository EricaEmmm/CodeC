#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) (a) > (b) ? (a) : (b)
#define MIN(a, b) (a) < (b) ? (a) : (b)

/*
qsort函数原型
void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*))
参数
base-- 指向要排序的数组的第一个元素的指针。
nitems-- 由 base 指向的数组中元素的个数。
size-- 数组中每个元素的大小，以字节为单位。
compar-- 用来比较两个元素的函数，即函数指针（回调函数）
    如果compar返回值小于0（< 0），那么p1所指向元素会被排在p2所指向元素的左面；
    如果compar返回值等于0（= 0），那么p1所指向元素与p2所指向元素的顺序不确定；
    如果compar返回值大于0（> 0），那么p1所指向元素会被排在p2所指向元素的右面。
注意compar函数两个形参必须是const void *型
（void *为无类型指针，可以指向任意类型的值，但它指向的值必须是常量）
在compar函数内部会将const void *型转换成实际类型
*/

/* 一维数组 升序排序 */
int CompOne(const void *a, const void *b)
{
    /*
    int ta = *(int *)a;
    int tb = *(int *)b;
    return ta > tb;
    */
    return *(int *)a - *(int *)b;
}

/* 二维数组 升序排序 */
int CompTwo(const void *a, const void *b)
{
    int *ta = (int *)a;
    int *tb = (int *)b;
    if(*ta != *tb)
        return *ta > *tb;
    return *(ta+1) > *(tb+1); 
} 

/* 字符串数组 升序排序 */
int CompStr(const void *a, const void *b)
{
    char *ta = *(char**)a;
    char *tb = *(char**)b;
    return strcmp(ta, tb); 
}


void CompOneTest(void)
{
    int array[] = {14, 3, 5, 23, 6};

    qsort(array, sizeof(array) / sizeof(array[0]), sizeof(int), CompOne);

    printf("oneArray:\n");
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void CompTwoTest(void)
{
    int array[][4]={{1,6,4},{4,1,5},{3,9,2},{3,9,1},{3,9,1,3},{3,9,1,2},{3,8,2}};
    //qsort(array,6,sizeof(array[0]),Cmp2);
    qsort(array, sizeof(array) / sizeof(array[0]), sizeof(array[0]), CompTwo);

    printf("twoArray:\n");
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        for (int j = 0; j < sizeof(array[0]) / sizeof(array[0][0]); j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void CompStrTest(void)
{
    char *array[] = {"abcd", "abc", "abca", "zzz", "ccc"};

    qsort(array, sizeof(array) / sizeof(array[0]), sizeof(char *), CompStr);

    printf("ptrArray:\n");
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        printf("%s ", array[i]);
    }
    printf("\n");
}

int main()
{
    CompOneTest();
    CompTwoTest();
    CompStrTest();

    system("pause");
    return 0;
}