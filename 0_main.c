#include <stdio.h>
#include <stdlib.h>
#include "include/hhash2.h"

/*
给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的那两个整数，并返回他们的数组下标。
你可以假设每种输入只会对应一个答案。但是，你不能重复利用这个数组中同样的元素。

示例:
给定 nums = [2, 7, 11, 15], target = 9
因为 nums[0] + nums[1] = 2 + 7 = 9
所以返回 [0, 1]
*/

//法一：暴力搜索
//时间复杂度：O(n^2)，空间复杂度：O(1)
int* twoSum1(int* nums, int numsSize, int target, int* returnSize)
{
    int *res = (int *)malloc(sizeof(int) * 2);
    for (int i = 0; i < numsSize; i++)
    {
        for (int j = i + 1; j < numsSize; j++)
        {
            if (nums[i] + nums[j] == target)
            {
                res[0] = i;
                res[1] = j;
                *returnSize = 2;
                return res;
            }
        }
    }
    *returnSize = 1;
    return 0;
}

//法二：Hash表
//时间复杂度：O(n)，空间复杂度：O(n)
int* twoSum2(int* nums, int numsSize, int target, int* returnSize)
{
    IntHash *IntHashTable = IntHashCreate();
    int *res;
    int flag;
    for (int i = 0; i < numsSize; i++)
    {
        if (IntHashExist(IntHashTable, target - nums[i]))
        {
            res[1] = i;
            res[0] = IntHashGet(IntHashTable, target - nums[i], NULL);
            *returnSize = 2;
            return res;
        }
        else
        {
            flag = IntHashSet(IntHashTable, nums[i], i);
        }
    }
    *returnSize = 1;
    return res;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    return twoSum1(nums, numsSize, target, returnSize);
}


int main()
{
    int nums[] = {2, 7, 11, 15};
    int target = 9;
    int returnSize;

    int *res;
    res = twoSum(nums, sizeof(nums) / sizeof(nums[0]), target, &returnSize);
    if (returnSize == 1)
        printf("not found\n");
    else
        printf("[%d, %d]\n", res[0], res[1]);
    system("pause");
    return 1;
}