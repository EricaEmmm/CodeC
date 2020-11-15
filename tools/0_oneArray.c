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