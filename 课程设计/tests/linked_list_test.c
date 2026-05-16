#include "linked_list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void free_int(void* data) {
    free((int*)data);
}

static void test_push_and_get(void) {
    LinkedList* list = linked_list_create(free_int);
    assert(list != NULL);

    int* a = (int*)malloc(sizeof(int));
    int* b = (int*)malloc(sizeof(int));
    int* c = (int*)malloc(sizeof(int));
    assert(a && b && c);
    *a = 10;
    *b = 20;
    *c = 30;

    assert(linked_list_push_back(list, a) == 1);
    assert(linked_list_push_back(list, b) == 1);
    assert(linked_list_push_back(list, c) == 1);

    assert(linked_list_size(list) == 3);
    assert(*(int*)linked_list_get(list, 0) == 10);
    assert(*(int*)linked_list_get(list, 1) == 20);
    assert(*(int*)linked_list_get(list, 2) == 30);
    assert(linked_list_get(list, 3) == NULL);

    linked_list_destroy(list);
}

static void test_remove_at(void) {
    LinkedList* list = linked_list_create(free_int);
    assert(list != NULL);

    for (int i = 1; i <= 5; ++i) {
        int* v = (int*)malloc(sizeof(int));
        assert(v != NULL);
        *v = i;
        assert(linked_list_push_back(list, v) == 1);
    }

    int* mid = (int*)linked_list_remove_at(list, 2);
    assert(mid != NULL && *mid == 3);
    free(mid);

    int* head = (int*)linked_list_remove_at(list, 0);
    assert(head != NULL && *head == 1);
    free(head);

    int* tail = (int*)linked_list_remove_at(list, linked_list_size(list) - 1);
    assert(tail != NULL && *tail == 5);
    free(tail);

    assert(linked_list_size(list) == 2);
    assert(*(int*)linked_list_get(list, 0) == 2);
    assert(*(int*)linked_list_get(list, 1) == 4);
    assert(linked_list_remove_at(list, 2) == NULL);

    linked_list_destroy(list);
}

int main(void) {
    test_push_and_get();
    test_remove_at();
    puts("linked_list tests passed");
    return 0;
}
