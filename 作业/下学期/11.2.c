#include <stdio.h>
#include <stdlib.h>

typedef struct Student {
    int id;
    char name[32];
    int score;
    struct Student *next;
} Student;

Student *input_list(int n) {
    Student *head = NULL, *tail = NULL, *node;
    int i;
    for (i = 0; i < n; i++) {
        node = (Student *)malloc(sizeof(Student));
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

Student *insert_ordered(Student *head, Student *node) {
    Student *p;
    if (head == NULL || node->score < head->score) {
        node->next = head;
        return node;
    }
    p = head;
    while (p->next != NULL && p->next->score <= node->score) {
        p = p->next;
    }
    node->next = p->next;
    p->next = node;
    return head;
}

void print_list(Student *head) {
    Student *p = head;
    while (p != NULL) {
        printf("%d %s %d\n", p->id, p->name, p->score);
        p = p->next;
    }
}

int main(void) {
    Student *head = input_list(5);
    Student *node = (Student *)malloc(sizeof(Student));
    scanf("%d %31s %d", &node->id, node->name, &node->score);
    node->next = NULL;

    head = insert_ordered(head, node);
    print_list(head);
    return 0;
}

