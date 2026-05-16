#ifndef ROOM_TYPE_H
#define ROOM_TYPE_H

#include <stddef.h>

typedef struct RoomType RoomType;
typedef struct RoomTypeRegistry RoomTypeRegistry;

RoomType* room_type_create(int id, const char* type_name);
void room_type_destroy(RoomType* room_type);

int room_type_get_id(const RoomType* room_type);
const char* room_type_get_name(const RoomType* room_type);
int room_type_set_name(RoomType* room_type, const char* type_name);

RoomTypeRegistry* room_type_registry_create(void);
void room_type_registry_destroy(RoomTypeRegistry* registry);
void room_type_registry_clear(RoomTypeRegistry* registry);

int room_type_registry_register(RoomTypeRegistry* registry, RoomType* room_type);
RoomType* room_type_registry_find_by_id(const RoomTypeRegistry* registry, int id);
int room_type_registry_remove_by_id(RoomTypeRegistry* registry, int id);

size_t room_type_registry_size(const RoomTypeRegistry* registry);
RoomType* room_type_registry_get_at(const RoomTypeRegistry* registry, size_t index);

#endif
