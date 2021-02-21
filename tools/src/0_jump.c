#define RUN_TIMES 80000   /* 循环次数控制 */

int Swap(int* array, int arraySize)
{
    int result = 0;
    int tmp;
    int start = 0;

    for (int i = 0; i < arraySize; i++) {
        if (array[i] != array[array[i]]) {
            tmp = array[i];
            array[i] = array[tmp];
            array[tmp] = tmp;
            result++;
            i = start;
            /* 循环次数过多，直接退出循环，返回0. 循环次数可用二分法找到合适值。 */
            if (result > RUN_TIMES) {
                return 0;
            }
        }
        start = i;
    }
    return result;
}

tools\src\0_oneArray.c
#include <stdio.h>
#include <stdlib.h>

/* 将Test()修改为用例给出的接口函数 */
int* Test(int* nums, int numsSize, int* returnSize){
    int *result = NULL;

    result = malloc(numsSize * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < numsSize; i++) {
        result[i] = i;
    }

    *returnSize = numsSize;
    return result;
}

int main()
{
    /* 此处需要根据用例的输入进行修改 */
    int nums[] = {1,2,1};

    int numsSize = sizeof(nums) / sizeof(int);
    int *result;
    int returnSize;

    result = Test(nums, numsSize, &returnSize);

    for (int i = 0; i < returnSize; i++) {
        printf("%d ", result[i]);
    }
    free(result);

    system("pause");
    return 0;
}