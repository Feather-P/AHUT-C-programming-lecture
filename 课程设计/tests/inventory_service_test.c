#include "../src/service/inventory_service.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

const char* config_get_room_file_path(void) { return "./room_test.json"; }
const char* config_get_room_type_file_path(void) { return "./room_type_test.json"; }
int room_repository_save_all_to_file(const RoomRegistry* registry, const char* file_path) {
    (void)registry;
    (void)file_path;
    return 1;
}
int room_type_repository_save_all_to_file(const RoomTypeRegistry* registry, const char* file_path) {
    (void)registry;
    (void)file_path;
    return 1;
}

int main(void) {
    RoomRegistry* rr = room_registry_create();
    RoomTypeRegistry* tr = room_type_registry_create();
    InventoryService svc;
    RoomType* rt = room_type_create(1, "标准间");
    Room* created_room;
    int created_room_id;
    CreateRoomCmd cmd;
    assert(rr && tr && rt);
    assert(room_type_registry_register(tr, rt) == 1);
    assert(inventory_service_init(&svc, rr, tr) == SERVICE_OK);
    memset(&cmd, 0, sizeof(cmd));
    snprintf(cmd.room_no, sizeof(cmd.room_no), "%s", "101");
    snprintf(cmd.description, sizeof(cmd.description), "%s", "ok");
    cmd.floor = 1;
    cmd.bed_count = 2;
    cmd.area = 30.0;
    cmd.type_id = 1;
    assert(inventory_service_create_room(&svc, &cmd) == SERVICE_OK);
    created_room = room_registry_get_at(rr, 0);
    assert(created_room != NULL);
    created_room_id = room_get_id(created_room);
    assert(inventory_service_mark_reserved(&svc, created_room_id) == SERVICE_OK);
    assert(inventory_service_mark_occupied(&svc, created_room_id) == SERVICE_OK);
    assert(inventory_service_mark_vacant(&svc, created_room_id) == SERVICE_OK);
    room_registry_destroy(rr);
    room_type_registry_destroy(tr);
    puts("inventory service tests passed");
    return 0;
}
