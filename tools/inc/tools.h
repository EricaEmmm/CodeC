#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>

/* 链表 */
struct ListNode {
    int val;
    struct ListNode *next;
};

// 创建链表
static inline struct ListNode* createList(int* nums, int numsSize) {
    // pre

    // init
    struct ListNode *dummy = (struct ListNode *)malloc(sizeof(struct ListNode));   // 创建哑节点
    struct ListNode *cur = dummy, *head = dummy;
    dummy->next = NULL;

    // todo
    for (int i = 0; i < numsSize; i ++) {
        struct ListNode *tmp = (struct ListNode *)malloc(sizeof(struct ListNode));
        tmp->val = nums[i];
        tmp->next = NULL;

        cur->next = tmp;
        cur = cur->next;
    }

    head = head->next;
    free(dummy);    // 释放哑结点，防止内存泄漏
    return head;
}

// 打印链表
static inline void printList(struct ListNode* node) {
    while (node) {
        printf("%d -> ", node->val);
        node = node->next;
    }
    printf("NULL\n");
}

#endif /* TOOLS_H */