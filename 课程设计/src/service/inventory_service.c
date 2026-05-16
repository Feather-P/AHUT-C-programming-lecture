#include "inventory_service.h"

#include "../config/config.h"
#include "../repository/repository.h"
#include "../utils/logger.h"

#include <time.h>

static ServiceCode inventory_service_set_status(InventoryService* svc, int room_id, RoomStatus to) {
    Room* room;
    RoomStatus from;
    if (svc == NULL || svc->room_registry == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    room = room_registry_find_by_id(svc->room_registry, room_id);
    if (room == NULL) {
        return SERVICE_ERR_NOT_FOUND;
    }
    from = room_get_status(room);
    if ((from == ROOM_STATUS_IDLE && (to == ROOM_STATUS_RESERVED || to == ROOM_STATUS_MAINTENANCE)) ||
        (from == ROOM_STATUS_RESERVED && (to == ROOM_STATUS_OCCUPIED || to == ROOM_STATUS_IDLE)) ||
        (from == ROOM_STATUS_OCCUPIED && to == ROOM_STATUS_IDLE) ||
        (from == ROOM_STATUS_MAINTENANCE && to == ROOM_STATUS_IDLE)) {
        room_set_status(room, to);
        room_set_updated_at(room, time(NULL));
        return SERVICE_OK;
    }
    return SERVICE_ERR_INVALID_STATE;
}

ServiceCode inventory_service_init(InventoryService* svc, RoomRegistry* room_registry, RoomTypeRegistry* room_type_registry) {
    if (svc == NULL || room_registry == NULL || room_type_registry == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    svc->room_registry = room_registry;
    svc->room_type_registry = room_type_registry;
    return SERVICE_OK;
}

ServiceCode inventory_service_add_room(InventoryService* svc, Room* room) {
    if (svc == NULL || svc->room_registry == NULL || room == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    if (room_registry_find_by_id(svc->room_registry, room_get_id(room)) != NULL) {
        return SERVICE_ERR_CONFLICT;
    }
    if (!room_registry_register(svc->room_registry, room)) {
        return SERVICE_ERR_INTERNAL;
    }
    LOG_INFO("inventory_service_add_room: persist rooms to '%s'", config_get_room_file_path());
    if (!room_repository_save_all_to_file(svc->room_registry, config_get_room_file_path())) {
        LOG_ERROR("inventory_service_add_room: persist failed path='%s'", config_get_room_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    LOG_INFO("inventory_service_add_room: persist success");
    return SERVICE_OK;
}

ServiceCode inventory_service_update_room(InventoryService* svc, const Room* room) {
    Room* target;
    if (svc == NULL || svc->room_registry == NULL || room == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    target = room_registry_find_by_id(svc->room_registry, room_get_id(room));
    if (target == NULL) {
        return SERVICE_ERR_NOT_FOUND;
    }
    if (room_get_no(room) != NULL && !room_set_no(target, room_get_no(room))) return SERVICE_ERR_INTERNAL;
    room_set_floor(target, room_get_floor(room));
    room_set_bed_count(target, room_get_bed_count(room));
    room_set_area(target, room_get_area(room));
    if (room_get_description(room) != NULL && !room_set_description(target, room_get_description(room))) return SERVICE_ERR_INTERNAL;
    room_set_type_id(target, room_get_type_id(room));
    room_set_status(target, room_get_status(room));
    room_set_updated_at(target, time(NULL));
    LOG_INFO("inventory_service_update_room: persist rooms to '%s'", config_get_room_file_path());
    if (!room_repository_save_all_to_file(svc->room_registry, config_get_room_file_path())) {
        LOG_ERROR("inventory_service_update_room: persist failed path='%s'", config_get_room_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    return SERVICE_OK;
}

ServiceCode inventory_service_remove_room(InventoryService* svc, int room_id) {
    if (svc == NULL || svc->room_registry == NULL) return SERVICE_ERR_INVALID_ARG;
    if (!room_registry_remove_by_id(svc->room_registry, room_id)) return SERVICE_ERR_NOT_FOUND;
    LOG_INFO("inventory_service_remove_room: persist rooms to '%s'", config_get_room_file_path());
    if (!room_repository_save_all_to_file(svc->room_registry, config_get_room_file_path())) {
        LOG_ERROR("inventory_service_remove_room: persist failed path='%s'", config_get_room_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    return SERVICE_OK;
}
ServiceCode inventory_service_mark_reserved(InventoryService* svc, int room_id) { return inventory_service_set_status(svc, room_id, ROOM_STATUS_RESERVED); }
ServiceCode inventory_service_mark_occupied(InventoryService* svc, int room_id) { return inventory_service_set_status(svc, room_id, ROOM_STATUS_OCCUPIED); }
ServiceCode inventory_service_mark_vacant(InventoryService* svc, int room_id) { return inventory_service_set_status(svc, room_id, ROOM_STATUS_IDLE); }
ServiceCode inventory_service_mark_maintenance(InventoryService* svc, int room_id) { return inventory_service_set_status(svc, room_id, ROOM_STATUS_MAINTENANCE); }

ServiceCode inventory_service_list_available_by_type(InventoryService* svc, int room_type_id, RoomRegistry* out_rooms) {
    size_t i, n;
    if (svc == NULL || svc->room_registry == NULL || out_rooms == NULL) return SERVICE_ERR_INVALID_ARG;
    n = room_registry_size(svc->room_registry);
    for (i = 0; i < n; ++i) {
        Room* room = room_registry_get_at(svc->room_registry, i);
        if (room != NULL && room_get_type_id(room) == room_type_id && room_get_status(room) == ROOM_STATUS_IDLE) {
            room_registry_register(out_rooms, room);
        }
    }
    return SERVICE_OK;
}
