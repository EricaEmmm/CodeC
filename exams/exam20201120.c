#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tools/inc/tools.h"
#include "tools/inc/test.h"

/*
自动售货机进出货管理系统
    工作级第3题/专业级第1题
请设计一款自动售货机的进出货管理系统 VendingMachineSystem()，需实现以下功能：
VendingMachineSystem(int trayNum, int trayCapacity)：设置该自动售货机上共有 trayNum 个商品轨道，每个商品轨道上最多可放置 trayCapacity 个商品。
    每个轨道上只能放置同一品牌的商品；同一品牌的商品只能放置在一个轨道上；
    对于已售空的轨道可以加入某一品牌的商品，即不一定是原品牌。
addProduct(int brandId, int[] productIdList)：向系统中添加商品，品牌为 brandId ，商品编号为 productIdList ，从所在轨道的末端商品之后按数组 productIdList 下标升序依次放入。
    售货机内已有该品牌的商品：若所在轨道剩余空间充足，则放入商品并返回 true ；否则不做任何操作并返回 false ；
    售货机内没有该品牌商品：若售货机内有空轨道，且该轨道空间充足，则放入商品并返回 true ；否则不做任何操作并返回 false 。
buyProduct(int brandId, int num)：购买品牌为 brandId 的 num 个商品。
    若有足够的商品，则取该轨道上前 num 个商品并返回其商品编号列表；
    若没有该品牌商品，或该商品数量不足 num 个，则不做任何操作并返回空数组 []。
queryProduct()：按照品牌编号升序返回每个品牌的首个商品编号(忽略空轨道)；若售货机为空，返回空数组 []。
【分析】

*/

#define MAX_NUM 200

typedef struct {
    int trayNum;        // 轨道数
    int trayCapacity;   // 轨道容量
    int trayUsed;       // 已使用轨道数
    int tray[MAX_NUM][MAX_NUM];    // 轨道数组, 以brandId为索引
    int trayProductNum[MAX_NUM];   // 每个轨道商品数数组
} VendingMachineSystem;

VendingMachineSystem* VendingMachineSystemCreate(int trayNum, int trayCapacity)
{
    VendingMachineSystem *obj = (VendingMachineSystem *)calloc(1, sizeof(VendingMachineSystem));
    obj->trayNum = trayNum;
    obj->trayCapacity = trayCapacity;

    return obj;
}

bool VendingMachineSystemAddProduct(VendingMachineSystem* obj, int brandId, int* productIdList, int productIdListSize)
{
    // 轨道数不足
    if ((obj->trayProductNum[brandId] == 0) && (obj->trayUsed >= obj->trayNum)) {
        return false;
    }
    // 轨道剩余空间不足
    if ((obj->trayProductNum[brandId] + productIdListSize) > obj->trayCapacity) {
        return false;
    }

    // 已使用轨道数+1
    if (obj->trayProductNum[brandId] == 0) {
        obj->trayUsed += 1;
    }

    // 将商品放到对应轨道上
    for (int i = 0; i < productIdListSize; i++) {
        obj->tray[brandId][obj->trayProductNum[brandId] + i] = productIdList[i];
    }
    obj->trayProductNum[brandId] += productIdListSize;

    return true;
}

/*
 * 注意：返回的数组必须在给定的函数内进行内存分配，由框架代码进行内存释放。返回的数组长度存在 *retSize 中。
 */
int* VendingMachineSystemBuyProduct(VendingMachineSystem* obj, int brandId, int num, int* retSize)
{
    // 商品数不足
    if (obj->trayProductNum[brandId] < num) {
        *retSize = 0;
        return NULL;
    }

    int *ret = (int *)malloc(sizeof(int) * num);
    
    // 取出商品
    for (int i = 0; i < num; i++) {
        ret[i] = obj->tray[brandId][i];
    }
    // 剩余商品前移
    for (int i = num; i < obj->trayProductNum[brandId]; i++) {
        obj->tray[brandId][i-num] = obj->tray[brandId][i];
    }

    obj->trayProductNum[brandId] -= num;
    if (obj->trayProductNum[brandId] == 0) {
        obj->trayUsed -= 1;
    }

    *retSize = num;
    return ret;
}

/*
 * 注意：返回的数组必须在给定的函数内进行内存分配，由框架代码进行内存释放。返回的数组长度存在 *retSize 中。
 */
int* VendingMachineSystemQueryProduct(VendingMachineSystem* obj, int* retSize)
{
    if (obj->trayUsed == 0) {
        *retSize = 0;
        return NULL;
    }

    // 获取商品种类
    int trayCnt = 0;
    for (int i = 0; i < MAX_NUM; i++) {
        if (obj->trayProductNum[i] > 0) {
            trayCnt += 1;
        }
    }

    int *ret = (int *)malloc(sizeof(int) * trayCnt);
    int j = 0;
    for (int i = 0; i < MAX_NUM; i++) {
        if (obj->trayProductNum[i] > 0) {
            ret[j++] = obj->tray[i][0];
        }
    }

    *retSize = trayCnt;
    return ret;
}

void VendingMachineSystemFree(VendingMachineSystem* obj)
{
    free(obj);
}


int main()
{
    TEST_FUNC_BEGIN();

    // test1
    VendingMachineSystem* obj = VendingMachineSystemCreate(2, 5);
    int list1[] = {3, 5, 4, 6, 2};
    SHOULD(VendingMachineSystemAddProduct(obj, 3, list1, 5) == true);
    int len = 0;
    int *product;
    product = VendingMachineSystemBuyProduct(obj, 3, 3, &len);
    for (int i = 0; i < len; i++) {
        printf("%d \n", product[i]);
    }
    free(product);
    product = VendingMachineSystemQueryProduct(obj, &len);
    for (int i = 0; i < len; i++) {
        printf("%d \n", product[i]);
    }
    free(product);
    VendingMachineSystemFree(obj);

    // test2
    obj = VendingMachineSystemCreate(2, 5);
    int list2[] = {3, 5, 4, 6, 2, 1};
    int list3[] = {9};
    int list4[] = {7};
    int list5[] = {10, 20, 13, 14};
    int list6[] = {10, 20, 13};
    int list7[] = {5, 4};
    SHOULD(VendingMachineSystemAddProduct(obj, 3, list2, 6) == false);
    SHOULD(VendingMachineSystemAddProduct(obj, 3, list1, 5) == true);
    SHOULD(VendingMachineSystemAddProduct(obj, 1, list3, 1) == true);
    SHOULD(VendingMachineSystemAddProduct(obj, 2, list4, 1) == false);
    product = VendingMachineSystemBuyProduct(obj, 3, 3, &len);
    for (int i = 0; i < len; i++) {
        printf("%d \n", product[i]);
    }
    free(product);
    product = VendingMachineSystemBuyProduct(obj, 9, 3, &len);
    for (int i = 0; i < len; i++) {
        printf("%d \n", product[i]);
    }
    free(product);
    product = VendingMachineSystemBuyProduct(obj, 3, 3, &len);
    for (int i = 0; i < len; i++) {
        printf("%d \n", product[i]);
    }
    free(product);
    product = VendingMachineSystemQueryProduct(obj, &len);
    for (int i = 0; i < len; i++) {
        printf("%d \n", product[i]);
    }
    free(product);
    SHOULD(VendingMachineSystemAddProduct(obj, 3, list5, 4) == false);
    SHOULD(VendingMachineSystemAddProduct(obj, 3, list6, 3) == true);
    product = VendingMachineSystemBuyProduct(obj, 3, 5, &len);
    for (int i = 0; i < len; i++) {
        printf("%d \n", product[i]);
    }
    free(product);
    SHOULD(VendingMachineSystemAddProduct(obj, 9, list7, 2) == true);
    VendingMachineSystemFree(obj);

    TEST_FUNC_END();

    system("pause");
    return 0;
}