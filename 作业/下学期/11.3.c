#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int id;
    char name[32];
    int score;
    struct ListNode *next;
} ListNode;

ListNode *input_list(int n) {
    ListNode *head = NULL, *tail = NULL, *node;
    int i;
    for (i = 0; i < n; i++) {
        node = (ListNode *)malloc(sizeof(ListNode));
        scanf("%d %31s %d", &node->id, node->name, &node->score);
        node->next = NULL;
        if (head == NULL) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode *prev = NULL, *cur = head, *next;
    while (cur != NULL) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

void print_list(ListNode *head) {
    ListNode *p = head;
    while (p != NULL) {
        printf("%d %s %d\n", p->id, p->name, p->score);
        p = p->next;
    }
}

int main(void) {
    ListNode *head = input_list(5);
    head = reverseList(head);
    print_list(head);
    return 0;
}

