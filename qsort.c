#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
*/

/*
定义比较的函数：一维数组
注意compar函数两个形参必须是const void *型
（void *为无类型指针，可以指向任意类型的值，但它指向的值必须是常量）
在compar函数内部会将const void *型转换成实际类型
升序排列
*/
int comp_up(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

/*
定义比较的函数：二维数组
注意compar函数两个形参必须是const void *型
升序排列
*/
int compArray1_up(const void *a, const void *b)
{
    // int *ap = *(int **)a;   //入参是数组元素的指针，此处数组元素也是指针
    // int *bp = *(int **)b;   
    // return ap[0] - bp[0];   //按照子数组第一个元素排序

    return ((int **)a)[0] - ((int **)b)[0]; //与上等价
}
int comArray2_up(const void *a, const void *b)
{
    return ((int *)a)[0] - ((int *)b)[0];
}

/*
定义比较的函数：字符串数组
注意compar函数两个形参必须是const void *型
字典序升序排列
*/
int compStr_up(const void *a, const void *b)
{
    return strcmp((char *)a, (char *)b);
}


int main()
{
    //一维数组排序
    int nums[5] = {3, 2, 3, 5, 7};
    qsort(nums, 5, sizeof(int), comp_up);
    for (int i = 0; i < 5; i++)
        printf("%d ", nums[i]);

    //二维数组
    //malloc申请的多维数组
    int **arrayInt = (int **)malloc(sizeof(int *) * 10);    //对int *来分配
    for (int i = 0; i < 10; i++)    //一次malloc后再来一次for循环malloc
    {
        arrayInt[i] = (int *)malloc(sizeof(int) * 2);
        arrayInt[i][0] = 100 - i;
        arrayInt[i][1] = 1 << i;
    }
    qsort(arrayInt, 10, sizeof(arrayInt[0]), compArray1_up);
    for (int i = 0; i < 10; i++)
        printf("%d ", arrayInt[i][0]);
    for (int i = 0; i < 10; i++)    //不能直接free(a)，因为malloc和free要一一对应
        free(arrayInt[i]);
    free(arrayInt);

    //二维数组
    //非malloc申请的多维数组
    int a[3][2] = {{2,3}, {1,4}, {4,8}};
    qsort(a, 3, sizeof(a[0]), comArray2_up);
    for (int i = 0; i < 3; i++)
        printf("%d ", a[i][0]);

    //字符串排序
    char s[3][4] = {"cad", "sbd", "abc"};
    qsort(s, 3, sizeof(s[0]), compStr_up);
    for (int i = 0; i < 3; i++)
        printf("%s ", s[i]);


    system("pause ");
    return 0;
}