#include "linked_list.h"

#include <stdlib.h>

struct LinkedListNode {
    struct LinkedListNode* prev;
    struct LinkedListNode* next;
    void* data;
};

struct LinkedList {
    LinkedListNode* head;
    LinkedListNode* tail;
    size_t size;
    linked_list_free_fn free_fn;
};

static LinkedListNode* linked_list_node_create(void* data) {
    LinkedListNode* node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    if (node == NULL) {
        return NULL;
    }

    node->prev = NULL;
    node->next = NULL;
    node->data = data;
    return node;
}

LinkedList* linked_list_create(linked_list_free_fn free_fn) {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->free_fn = free_fn;
    return list;
}

void linked_list_clear(LinkedList* list) {
    if (list == NULL) {
        return;
    }

    LinkedListNode* current = list->head;
    while (current != NULL) {
        LinkedListNode* next = current->next;
        if (list->free_fn != NULL && current->data != NULL) {
            list->free_fn(current->data);
        }
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void linked_list_destroy(LinkedList* list) {
    if (list == NULL) {
        return;
    }

    linked_list_clear(list);
    free(list);
}

int linked_list_push_back(LinkedList* list, void* data) {
    if (list == NULL) {
        return 0;
    }

    LinkedListNode* node = linked_list_node_create(data);
    if (node == NULL) {
        return 0;
    }

    node->prev = list->tail;
    if (list->tail != NULL) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
    return 1;
}

int linked_list_push_front(LinkedList* list, void* data) {
    if (list == NULL) {
        return 0;
    }

    LinkedListNode* node = linked_list_node_create(data);
    if (node == NULL) {
        return 0;
    }

    node->next = list->head;
    if (list->head != NULL) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }
    list->head = node;
    list->size++;
    return 1;
}

void* linked_list_pop_back(LinkedList* list) {
    if (list == NULL || list->tail == NULL) {
        return NULL;
    }

    LinkedListNode* node = list->tail;
    void* data = node->data;

    list->tail = node->prev;
    if (list->tail != NULL) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    free(node);
    list->size--;
    return data;
}

void* linked_list_pop_front(LinkedList* list) {
    if (list == NULL || list->head == NULL) {
        return NULL;
    }

    LinkedListNode* node = list->head;
    void* data = node->data;

    list->head = node->next;
    if (list->head != NULL) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(node);
    list->size--;
    return data;
}

void* linked_list_get(const LinkedList* list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }

    LinkedListNode* current;
    size_t i;

    if (index <= list->size / 2) {
        current = list->head;
        for (i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = list->tail;
        for (i = list->size - 1; i > index; --i) {
            current = current->prev;
        }
    }

    return current->data;
}

void* linked_list_remove_at(LinkedList* list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }

    if (index == 0) {
        return linked_list_pop_front(list);
    }
    if (index == list->size - 1) {
        return linked_list_pop_back(list);
    }

    LinkedListNode* current;
    size_t i;

    if (index <= list->size / 2) {
        current = list->head;
        for (i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = list->tail;
        for (i = list->size - 1; i > index; --i) {
            current = current->prev;
        }
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    void* data = current->data;
    free(current);
    list->size--;
    return data;
}

size_t linked_list_size(const LinkedList* list) {
    return list == NULL ? 0 : list->size;
}

int linked_list_is_empty(const LinkedList* list) {
    return linked_list_size(list) == 0;
}
