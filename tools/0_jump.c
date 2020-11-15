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