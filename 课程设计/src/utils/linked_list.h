#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

typedef void (*linked_list_free_fn)(void* data);

typedef struct LinkedListNode LinkedListNode;
typedef struct LinkedList LinkedList;

LinkedList* linked_list_create(linked_list_free_fn free_fn);
void linked_list_destroy(LinkedList* list);
void linked_list_clear(LinkedList* list);

int linked_list_push_back(LinkedList* list, void* data);
int linked_list_push_front(LinkedList* list, void* data);

void* linked_list_pop_back(LinkedList* list);
void* linked_list_pop_front(LinkedList* list);
void* linked_list_get(const LinkedList* list, size_t index);
void* linked_list_remove_at(LinkedList* list, size_t index);

size_t linked_list_size(const LinkedList* list);
int linked_list_is_empty(const LinkedList* list);

#endif
