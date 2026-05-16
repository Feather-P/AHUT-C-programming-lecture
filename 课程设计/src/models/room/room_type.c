#include "room_type.h"

#include "../../utils/linked_list.h"

#include <stdlib.h>
#include <string.h>

struct RoomType {
    int id;
    char* type_name;
};

struct RoomTypeRegistry {
    LinkedList* room_types;
};

static char* room_type_strdup(const char* src) {
    if (src == NULL) {
        return NULL;
    }

    size_t len = strlen(src);
    char* dst = (char*)malloc(len + 1);
    if (dst == NULL) {
        return NULL;
    }

    memcpy(dst, src, len + 1);
    return dst;
}

RoomType* room_type_create(int id, const char* type_name) {
    RoomType* room_type = (RoomType*)malloc(sizeof(RoomType));
    if (room_type == NULL) {
        return NULL;
    }

    room_type->id = id;
    room_type->type_name = room_type_strdup(type_name);
    if (type_name != NULL && room_type->type_name == NULL) {
        free(room_type);
        return NULL;
    }

    return room_type;
}

void room_type_destroy(RoomType* room_type) {
    if (room_type == NULL) {
        return;
    }

    free(room_type->type_name);
    free(room_type);
}

int room_type_get_id(const RoomType* room_type) {
    if (room_type == NULL) {
        return -1;
    }
    return room_type->id;
}

const char* room_type_get_name(const RoomType* room_type) {
    if (room_type == NULL) {
        return NULL;
    }
    return room_type->type_name;
}

int room_type_set_name(RoomType* room_type, const char* type_name) {
    char* new_name;
    if (room_type == NULL) {
        return 0;
    }

    new_name = room_type_strdup(type_name);
    if (type_name != NULL && new_name == NULL) {
        return 0;
    }

    free(room_type->type_name);
    room_type->type_name = new_name;
    return 1;
}

RoomTypeRegistry* room_type_registry_create(void) {
    RoomTypeRegistry* registry = (RoomTypeRegistry*)malloc(sizeof(RoomTypeRegistry));
    if (registry == NULL) {
        return NULL;
    }

    registry->room_types = linked_list_create((linked_list_free_fn)room_type_destroy);
    if (registry->room_types == NULL) {
        free(registry);
        return NULL;
    }

    return registry;
}

void room_type_registry_clear(RoomTypeRegistry* registry) {
    if (registry == NULL || registry->room_types == NULL) {
        return;
    }

    linked_list_clear(registry->room_types);
}

void room_type_registry_destroy(RoomTypeRegistry* registry) {
    if (registry == NULL) {
        return;
    }

    linked_list_destroy(registry->room_types);
    free(registry);
}

RoomType* room_type_registry_find_by_id(const RoomTypeRegistry* registry, int id) {
    size_t i;
    size_t n;

    if (registry == NULL || registry->room_types == NULL) {
        return NULL;
    }

    n = linked_list_size(registry->room_types);
    for (i = 0; i < n; ++i) {
        RoomType* room_type = (RoomType*)linked_list_get(registry->room_types, i);
        if (room_type != NULL && room_type_get_id(room_type) == id) {
            return room_type;
        }
    }

    return NULL;
}

int room_type_registry_register(RoomTypeRegistry* registry, RoomType* room_type) {
    if (registry == NULL || registry->room_types == NULL || room_type == NULL) {
        return 0;
    }

    if (room_type_registry_find_by_id(registry, room_type_get_id(room_type)) != NULL) {
        return 0;
    }

    return linked_list_push_back(registry->room_types, room_type);
}

int room_type_registry_remove_by_id(RoomTypeRegistry* registry, int id) {
    size_t i;
    size_t n;

    if (registry == NULL || registry->room_types == NULL) {
        return 0;
    }

    n = linked_list_size(registry->room_types);
    for (i = 0; i < n; ++i) {
        RoomType* room_type = (RoomType*)linked_list_get(registry->room_types, i);
        if (room_type != NULL && room_type_get_id(room_type) == id) {
            RoomType* removed = (RoomType*)linked_list_remove_at(registry->room_types, i);
            room_type_destroy(removed);
            return 1;
        }
    }

    return 0;
}

size_t room_type_registry_size(const RoomTypeRegistry* registry) {
    if (registry == NULL || registry->room_types == NULL) {
        return 0;
    }
    return linked_list_size(registry->room_types);
}

RoomType* room_type_registry_get_at(const RoomTypeRegistry* registry, size_t index) {
    if (registry == NULL || registry->room_types == NULL) {
        return NULL;
    }
    return (RoomType*)linked_list_get(registry->room_types, index);
}
