#include <stdio.h>
#include <stdlib.h>

#define null 0xffffff

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

/* 将Test()修改为用例给出的接口函数 */
int Test(struct TreeNode *root)
{
    return 0;
}

int main()
{
    /* 用例输入的用一维数组表示的二叉树 */
    int nodeVal[] = {5, 4, 8, 11, null, 13, 4, 7, 2, null, null, 5, 1};
    int result;

    int nodeNum = sizeof(nodeVal) / sizeof(int);
    struct TreeNode *node[nodeNum];

    /* 申请测试用例输入参数中需要的内存并根据用例输入初始化 */
    for (int i = 0; i < nodeNum; i++) {
        node[i] = (struct TreeNode *)malloc(sizeof(struct TreeNode));
        node[i]->val = nodeVal[i];
        node[i]->right = NULL;
        node[i]->left = NULL;
    }

    /* 将用例的一维数组转换成二叉树 */
    int absentNum = 0;
    int left;
    int right;
    for (int i = 0; i < nodeNum; i++) {
        if (nodeVal[i] == null) {
            absentNum++;
            continue;
        }
        left = i * 2 - 2 * absentNum + 1;
        right = i * 2 - 2 * absentNum + 2;
        if (left < nodeNum) {
            node[i]->left = node[left];
        }
        if (right < nodeNum) {
            node[i]->right = node[right];
        }
    }

    /* 运行测试用例给出的接口函数 */
    result = Test(node[0]);
    printf("%d\n", result);

    /* 释放测试用例输入参数中申请的内存 */
    for (int i = 0; i < nodeNum; i++) {
        free(node[i]);
    }
    return 0;
}