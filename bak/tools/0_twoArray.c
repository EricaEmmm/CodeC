#include <stdio.h>
#include <stdlib.h>

/* 此处需要根据用例的输入进行修改 */
#define MAP_ROW 6
#define MAP_COL 2
int g_map[MAP_ROW][MAP_COL] = {{8,0}, {4,4}, {8,1}, {5,0}, {6,1}, {5,2}};

/* 将Test()修改为用例给出的接口函数 */
int** Test(int** grid, int gridSize, int* gridColSize, int* returnSize, int** returnColumnSizes)
{
    int retSize = gridSize;             /* 返回二维数组的行数 */
    int retColSize = gridColSize[0];    /* 返回二维数组的列数 */
    int **result = NULL;

    /* 申请返回的二维数组内存 */
    result = malloc(retSize * sizeof(int *));
    if (result == NULL) {
        return NULL;
    }
    *returnColumnSizes = malloc(retSize * sizeof(int));
    if (*returnColumnSizes == NULL) {
        return NULL;
    }

    /* todo */

    /* 对返回的二维数组赋值 */
    *returnSize = retSize;
    for (int i = 0; i < retSize; i++) {
        (*returnColumnSizes)[i] = retColSize;
        result[i] = malloc(retColSize * sizeof(int));
        if (result[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        for (int num = 0; num < retColSize; num++) {
            result[i][num] = num;   /* 此处填写运算后需要返回的结果 */
        }
    }
    return result;
}

int main()
{
    int gridSize = MAP_ROW;
    int **grid = NULL;
    int *gridColSize = NULL;
    int **result = NULL;
    int returnSize;
    int *returnColumnSizes = NULL;

    /* 申请测试用例输入参数中需要的内存并根据用例输入初始化 */
    // 申请 gridSize 行
    grid = malloc(gridSize * sizeof(int *));
    if (grid == NULL) {
        return -1;
    }
    // 每行 gridColSize 列
    gridColSize = malloc(gridSize * sizeof(int));
    if (gridColSize == NULL) {
        return -1;
    }
    // 对每行分配内存
    for (int i = 0; i < gridSize; i++) {
        grid[i] = malloc(MAP_COL * sizeof(int));
        if (grid[i] == NULL) {
            return -1;
        }
        gridColSize[i] = MAP_COL;
    }
    /* 将测试用例的输入参数转换成用例输入的二维数组 */
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridColSize[i]; j++) {
            grid[i][j] = g_map[i][j];
        }
    }

    /* 运行测试用例给出的接口函数 */
    result = Test(grid, gridSize, gridColSize, &returnSize, &returnColumnSizes);

    /* 释放测试函数中申请的内存 */
    for (int row = 0; row < returnSize; row++) {
        for (int col = 0; col < returnColumnSizes[row]; col++) {
            printf("%d ", result[row][col]);
        }
        printf("\n");
        free(result[row]);
    }
    free(returnColumnSizes);
    free(result);

    /* 释放测试用例输入参数中申请的内存 */
    for (int i = 0; i < gridSize; i++) {
        free(grid[i]);
    }
    free(gridColSize);
    free(grid);

    system("pause");
    return 0;
}