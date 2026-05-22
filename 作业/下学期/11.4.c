#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Student *delete_by_name(Student *head, const char *target, int *found) {
    Student *p = head, *prev = NULL;
    *found = 0;
    while (p != NULL) {
        if (strcmp(p->name, target) == 0) {
            *found = 1;
            if (prev == NULL) {
                head = p->next;
            } else {
                prev->next = p->next;
            }
            free(p);
            return head;
        }
        prev = p;
        p = p->next;
    }
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
    char target[32];
    int found;

    scanf("%31s", target);
    head = delete_by_name(head, target, &found);
    if (!found) {
        printf("Not Found\n");
    } else {
        print_list(head);
    }
    return 0;
}

