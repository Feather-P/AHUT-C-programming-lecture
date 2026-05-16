#include "../src/models/room/room.h"
#include "../src/models/room/room_type.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    RoomTypeRegistry* tr = room_type_registry_create();
    RoomRegistry* rr = room_registry_create();
    RoomType* t = room_type_create(1, "单人间");
    Room* r = room_create(101, "101", 1, 1, 20.0, "ok", 1, ROOM_STATUS_IDLE, 1, 1, 0);
    assert(tr && rr && t && r);
    assert(room_type_registry_register(tr, t) == 1);
    assert(room_registry_register(rr, r) == 1);
    assert(room_registry_find_by_id(rr, 101) == r);
    room_set_status(r, ROOM_STATUS_RESERVED);
    assert(room_get_status(r) == ROOM_STATUS_RESERVED);
    assert(room_registry_remove_by_id(rr, 101) == 1);
    room_registry_destroy(rr);
    room_type_registry_destroy(tr);
    puts("room model tests passed");
    return 0;
}
