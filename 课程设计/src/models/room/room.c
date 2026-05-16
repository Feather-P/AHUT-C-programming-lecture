#include "room.h"

#include "../../utils/linked_list.h"

#include <stdlib.h>
#include <string.h>

struct Room {
    int room_id;
    char* room_no;
    int floor;
    int bed_count;
    double area;
    char* description;
    int room_type_id;
    RoomStatus room_status;
    time_t created_at;
    time_t updated_at;
    time_t last_checkout_time;
};

struct RoomRegistry {
    LinkedList* rooms;
};

static char* room_strdup(const char* src) {
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

Room* room_create(int room_id,
                  const char* room_no,
                  int floor,
                  int bed_count,
                  double area,
                  const char* description,
                  int room_type_id,
                  RoomStatus room_status,
                  time_t created_at,
                  time_t updated_at,
                  time_t last_checkout_time) {
    Room* room = (Room*)malloc(sizeof(Room));
    if (room == NULL) {
        return NULL;
    }

    room->room_id = room_id;
    room->floor = floor;
    room->bed_count = bed_count;
    room->area = area;
    room->room_type_id = room_type_id;
    room->room_status = room_status;
    room->created_at = created_at;
    room->updated_at = updated_at;
    room->last_checkout_time = last_checkout_time;

    room->room_no = room_strdup(room_no);
    if (room_no != NULL && room->room_no == NULL) {
        free(room);
        return NULL;
    }

    room->description = room_strdup(description);
    if (description != NULL && room->description == NULL) {
        free(room->room_no);
        free(room);
        return NULL;
    }

    return room;
}

void room_destroy(Room* room) {
    if (room == NULL) {
        return;
    }

    free(room->room_no);
    free(room->description);
    free(room);
}

int room_get_id(const Room* room) {
    if (room == NULL) {
        return -1;
    }
    return room->room_id;
}

const char* room_get_no(const Room* room) {
    if (room == NULL) {
        return NULL;
    }
    return room->room_no;
}

int room_set_no(Room* room, const char* room_no) {
    char* new_room_no;
    if (room == NULL) {
        return 0;
    }

    new_room_no = room_strdup(room_no);
    if (room_no != NULL && new_room_no == NULL) {
        return 0;
    }

    free(room->room_no);
    room->room_no = new_room_no;
    return 1;
}

int room_get_floor(const Room* room) {
    if (room == NULL) {
        return -1;
    }
    return room->floor;
}

void room_set_floor(Room* room, int floor) {
    if (room == NULL) {
        return;
    }
    room->floor = floor;
}

int room_get_bed_count(const Room* room) {
    if (room == NULL) {
        return -1;
    }
    return room->bed_count;
}

void room_set_bed_count(Room* room, int bed_count) {
    if (room == NULL) {
        return;
    }
    room->bed_count = bed_count;
}

double room_get_area(const Room* room) {
    if (room == NULL) {
        return 0.0;
    }
    return room->area;
}

void room_set_area(Room* room, double area) {
    if (room == NULL) {
        return;
    }
    room->area = area;
}

const char* room_get_description(const Room* room) {
    if (room == NULL) {
        return NULL;
    }
    return room->description;
}

int room_set_description(Room* room, const char* description) {
    char* new_description;
    if (room == NULL) {
        return 0;
    }

    new_description = room_strdup(description);
    if (description != NULL && new_description == NULL) {
        return 0;
    }

    free(room->description);
    room->description = new_description;
    return 1;
}

int room_get_type_id(const Room* room) {
    if (room == NULL) {
        return -1;
    }
    return room->room_type_id;
}

void room_set_type_id(Room* room, int type_id) {
    if (room == NULL) {
        return;
    }
    room->room_type_id = type_id;
}

RoomStatus room_get_status(const Room* room) {
    if (room == NULL) {
        return ROOM_STATUS_IDLE;
    }
    return room->room_status;
}

void room_set_status(Room* room, RoomStatus room_status) {
    if (room == NULL) {
        return;
    }
    room->room_status = room_status;
}

time_t room_get_created_at(const Room* room) {
    if (room == NULL) {
        return (time_t)0;
    }
    return room->created_at;
}

void room_set_created_at(Room* room, time_t created_at) {
    if (room == NULL) {
        return;
    }
    room->created_at = created_at;
}

time_t room_get_updated_at(const Room* room) {
    if (room == NULL) {
        return (time_t)0;
    }
    return room->updated_at;
}

void room_set_updated_at(Room* room, time_t updated_at) {
    if (room == NULL) {
        return;
    }
    room->updated_at = updated_at;
}

time_t room_get_last_checkout_time(const Room* room) {
    if (room == NULL) {
        return (time_t)0;
    }
    return room->last_checkout_time;
}

void room_set_last_checkout_time(Room* room, time_t last_checkout_time) {
    if (room == NULL) {
        return;
    }
    room->last_checkout_time = last_checkout_time;
}

RoomRegistry* room_registry_create(void) {
    RoomRegistry* registry = (RoomRegistry*)malloc(sizeof(RoomRegistry));
    if (registry == NULL) {
        return NULL;
    }

    registry->rooms = linked_list_create((linked_list_free_fn)room_destroy);
    if (registry->rooms == NULL) {
        free(registry);
        return NULL;
    }

    return registry;
}

void room_registry_clear(RoomRegistry* registry) {
    if (registry == NULL || registry->rooms == NULL) {
        return;
    }

    linked_list_clear(registry->rooms);
}

void room_registry_destroy(RoomRegistry* registry) {
    if (registry == NULL) {
        return;
    }

    linked_list_destroy(registry->rooms);
    free(registry);
}

Room* room_registry_find_by_id(const RoomRegistry* registry, int room_id) {
    size_t i;
    size_t n;

    if (registry == NULL || registry->rooms == NULL) {
        return NULL;
    }

    n = linked_list_size(registry->rooms);
    for (i = 0; i < n; ++i) {
        Room* room = (Room*)linked_list_get(registry->rooms, i);
        if (room != NULL && room_get_id(room) == room_id) {
            return room;
        }
    }

    return NULL;
}

int room_registry_register(RoomRegistry* registry, Room* room) {
    if (registry == NULL || registry->rooms == NULL || room == NULL) {
        return 0;
    }

    if (room_registry_find_by_id(registry, room_get_id(room)) != NULL) {
        return 0;
    }

    return linked_list_push_back(registry->rooms, room);
}

int room_registry_remove_by_id(RoomRegistry* registry, int room_id) {
    size_t i;
    size_t n;

    if (registry == NULL || registry->rooms == NULL) {
        return 0;
    }

    n = linked_list_size(registry->rooms);
    for (i = 0; i < n; ++i) {
        Room* room = (Room*)linked_list_get(registry->rooms, i);
        if (room != NULL && room_get_id(room) == room_id) {
            Room* removed = (Room*)linked_list_remove_at(registry->rooms, i);
            room_destroy(removed);
            return 1;
        }
    }

    return 0;
}

size_t room_registry_size(const RoomRegistry* registry) {
    if (registry == NULL || registry->rooms == NULL) {
        return 0;
    }
    return linked_list_size(registry->rooms);
}

Room* room_registry_get_at(const RoomRegistry* registry, size_t index) {
    if (registry == NULL || registry->rooms == NULL) {
        return NULL;
    }
    return (Room*)linked_list_get(registry->rooms, index);
}
