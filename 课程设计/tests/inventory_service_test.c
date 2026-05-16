#include "../src/service/inventory_service.h"

#include <assert.h>
#include <stdio.h>

const char* config_get_room_file_path(void) { return "./room_test.json"; }
int room_repository_save_all_to_file(const RoomRegistry* registry, const char* file_path) {
    (void)registry;
    (void)file_path;
    return 1;
}

int main(void) {
    RoomRegistry* rr = room_registry_create();
    RoomTypeRegistry* tr = room_type_registry_create();
    InventoryService svc;
    RoomType* rt = room_type_create(1, "标准间");
    Room* r = room_create(101, "101", 1, 2, 30.0, "ok", 1, ROOM_STATUS_IDLE, 1, 1, 0);
    assert(rr && tr && rt && r);
    assert(room_type_registry_register(tr, rt) == 1);
    assert(inventory_service_init(&svc, rr, tr) == SERVICE_OK);
    assert(inventory_service_add_room(&svc, r) == SERVICE_OK);
    assert(inventory_service_mark_reserved(&svc, 101) == SERVICE_OK);
    assert(inventory_service_mark_occupied(&svc, 101) == SERVICE_OK);
    assert(inventory_service_mark_vacant(&svc, 101) == SERVICE_OK);
    room_registry_destroy(rr);
    room_type_registry_destroy(tr);
    puts("inventory service tests passed");
    return 0;
}
