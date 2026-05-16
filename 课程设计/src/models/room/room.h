#ifndef ROOM_H
#define ROOM_H

#include <stddef.h>
#include <time.h>

typedef struct Room Room;
typedef struct RoomRegistry RoomRegistry;

typedef enum RoomStatus {
    ROOM_STATUS_IDLE = 0,
    ROOM_STATUS_RESERVED = 1,
    ROOM_STATUS_OCCUPIED = 2,
    ROOM_STATUS_MAINTENANCE = 3
} RoomStatus;

Room* room_create(
    int room_id,
    const char* room_no,
    int floor,
    int bed_count,
    double area,
    const char* description,
    int room_type_id,
    RoomStatus room_status,
    time_t created_at,
    time_t updated_at,
    time_t last_checkout_time);
void room_destroy(Room* room);

int room_get_id(const Room* room);

const char* room_get_no(const Room* room);
int room_set_no(Room* room, const char* room_no);

int room_get_floor(const Room* room);
void room_set_floor(Room* room, int floor);

int room_get_bed_count(const Room* room);
void room_set_bed_count(Room* room, int bed_count);

double room_get_area(const Room* room);
void room_set_area(Room* room, double area);

const char* room_get_description(const Room* room);
int room_set_description(Room* room, const char* description);

int room_get_type_id(const Room* room);
void room_set_type_id(Room* room, int type_id);

RoomStatus room_get_status(const Room* room);
void room_set_status(Room* room, RoomStatus room_status);

time_t room_get_created_at(const Room* room);
void room_set_created_at(Room* room, time_t created_at);

time_t room_get_updated_at(const Room* room);
void room_set_updated_at(Room* room, time_t updated_at);

time_t room_get_last_checkout_time(const Room* room);
void room_set_last_checkout_time(Room* room, time_t last_checkout_time);

RoomRegistry* room_registry_create(void);
void room_registry_destroy(RoomRegistry* registry);
void room_registry_clear(RoomRegistry* registry);

int room_registry_register(RoomRegistry* registry, Room* room);
Room* room_registry_find_by_id(const RoomRegistry* registry, int room_id);
int room_registry_remove_by_id(RoomRegistry* registry, int room_id);

size_t room_registry_size(const RoomRegistry* registry);
Room* room_registry_get_at(const RoomRegistry* registry, size_t index);

#endif
