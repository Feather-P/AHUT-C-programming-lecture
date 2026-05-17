#include "inventory_service.h"

#include "../config/config.h"
#include "../repository/repository.h"
#include "../utils/logger.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

static int next_room_id(RoomRegistry* rr) {
    size_t i, n;
    int max_id = 0;
    n = room_registry_size(rr);
    for (i = 0; i < n; ++i) {
        Room* r = room_registry_get_at(rr, i);
        if (r != NULL && room_get_id(r) > max_id) max_id = room_get_id(r);
    }
    return max_id + 1;
}

static int persist_rooms(InventoryService* svc) {
    return room_repository_save_all_to_file(svc->room_registry, config_get_room_file_path());
}

static int persist_room_types(InventoryService* svc) {
    return room_type_repository_save_all_to_file(svc->room_type_registry, config_get_room_type_file_path());
}

static int is_room_type_in_use(InventoryService* svc, int room_type_id) {
    size_t i, n;
    n = room_registry_size(svc->room_registry);
    for (i = 0; i < n; ++i) {
        Room* room = room_registry_get_at(svc->room_registry, i);
        if (room != NULL && room_get_type_id(room) == room_type_id) return 1;
    }
    return 0;
}

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

ServiceCode inventory_service_create_room(InventoryService* svc, const CreateRoomCmd* cmd) {
    Room* room;
    int id;
    if (svc == NULL || svc->room_registry == NULL || cmd == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    if (room_type_registry_find_by_id(svc->room_type_registry, cmd->type_id) == NULL) {
        return SERVICE_ERR_NOT_FOUND;
    }
    id = next_room_id(svc->room_registry);
    room = room_create(id, cmd->room_no, cmd->floor, cmd->bed_count, cmd->area, cmd->description,
                       cmd->type_id, ROOM_STATUS_IDLE, time(NULL), time(NULL), 0);
    if (room == NULL) return SERVICE_ERR_INTERNAL;
    if (!room_registry_register(svc->room_registry, room)) {
        room_destroy(room);
        return SERVICE_ERR_INTERNAL;
    }
    LOG_INFO("inventory_service_create_room: persist rooms to '%s'", config_get_room_file_path());
    if (!persist_rooms(svc)) {
        LOG_ERROR("inventory_service_create_room: persist failed path='%s'", config_get_room_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    LOG_INFO("inventory_service_create_room: persist success");
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
    if (room_type_registry_find_by_id(svc->room_type_registry, room_get_type_id(room)) == NULL) {
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
    if (!persist_rooms(svc)) {
        LOG_ERROR("inventory_service_update_room: persist failed path='%s'", config_get_room_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    return SERVICE_OK;
}

ServiceCode inventory_service_remove_room(InventoryService* svc, int room_id) {
    if (svc == NULL || svc->room_registry == NULL) return SERVICE_ERR_INVALID_ARG;
    if (!room_registry_remove_by_id(svc->room_registry, room_id)) return SERVICE_ERR_NOT_FOUND;
    LOG_INFO("inventory_service_remove_room: persist rooms to '%s'", config_get_room_file_path());
    if (!persist_rooms(svc)) {
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

ServiceCode inventory_service_list_all_rooms(InventoryService* svc, RoomRegistry* out_rooms) {
    size_t i, n;
    if (svc == NULL || svc->room_registry == NULL || out_rooms == NULL) return SERVICE_ERR_INVALID_ARG;
    n = room_registry_size(svc->room_registry);
    for (i = 0; i < n; ++i) {
        Room* room = room_registry_get_at(svc->room_registry, i);
        if (room != NULL) {
            room_registry_register(out_rooms, room);
        }
    }
    return SERVICE_OK;
}

ServiceCode inventory_service_add_room_type(InventoryService* svc, RoomType* room_type) {
    if (svc == NULL || svc->room_type_registry == NULL || room_type == NULL) return SERVICE_ERR_INVALID_ARG;
    if (room_type_registry_find_by_id(svc->room_type_registry, room_type_get_id(room_type)) != NULL) return SERVICE_ERR_CONFLICT;
    if (!room_type_registry_register(svc->room_type_registry, room_type)) return SERVICE_ERR_INTERNAL;
    if (!persist_room_types(svc)) return SERVICE_ERR_INTERNAL;
    return SERVICE_OK;
}

ServiceCode inventory_service_update_room_type_name(InventoryService* svc, int room_type_id, const char* new_name) {
    RoomType* rt;
    if (svc == NULL || svc->room_type_registry == NULL || new_name == NULL) return SERVICE_ERR_INVALID_ARG;
    rt = room_type_registry_find_by_id(svc->room_type_registry, room_type_id);
    if (rt == NULL) return SERVICE_ERR_NOT_FOUND;
    if (is_room_type_in_use(svc, room_type_id)) return SERVICE_ERR_CONFLICT;
    if (!room_type_set_name(rt, new_name)) return SERVICE_ERR_INTERNAL;
    if (!persist_room_types(svc)) return SERVICE_ERR_INTERNAL;
    return SERVICE_OK;
}

ServiceCode inventory_service_remove_room_type(InventoryService* svc, int room_type_id) {
    if (svc == NULL || svc->room_type_registry == NULL) return SERVICE_ERR_INVALID_ARG;
    if (is_room_type_in_use(svc, room_type_id)) return SERVICE_ERR_CONFLICT;
    if (!room_type_registry_remove_by_id(svc->room_type_registry, room_type_id)) return SERVICE_ERR_NOT_FOUND;
    if (!persist_room_types(svc)) return SERVICE_ERR_INTERNAL;
    return SERVICE_OK;
}

ServiceCode inventory_service_get_room_type_name(InventoryService* svc, int room_type_id, char* out_name, size_t out_name_size) {
    RoomType* rt;
    const char* name;
    if (svc == NULL || svc->room_type_registry == NULL || out_name == NULL || out_name_size == 0) return SERVICE_ERR_INVALID_ARG;
    rt = room_type_registry_find_by_id(svc->room_type_registry, room_type_id);
    if (rt == NULL) return SERVICE_ERR_NOT_FOUND;
    name = room_type_get_name(rt);
    if (name == NULL) name = "";
    snprintf(out_name, out_name_size, "%s", name);
    return SERVICE_OK;
}

ServiceCode inventory_service_list_all_room_views(InventoryService* svc,
                                                  RoomView* out_views,
                                                  size_t max_views,
                                                  size_t* out_count) {
    size_t i, n, w = 0;
    if (svc == NULL || out_views == NULL || out_count == NULL) return SERVICE_ERR_INVALID_ARG;
    n = room_registry_size(svc->room_registry);
    for (i = 0; i < n && w < max_views; ++i) {
        Room* room = room_registry_get_at(svc->room_registry, i);
        if (room == NULL) continue;
        snprintf(out_views[w].room_no, sizeof(out_views[w].room_no), "%s", room_get_no(room) == NULL ? "" : room_get_no(room));
        out_views[w].floor = room_get_floor(room);
        out_views[w].bed_count = room_get_bed_count(room);
        out_views[w].area = room_get_area(room);
        out_views[w].type_id = room_get_type_id(room);
        out_views[w].status = room_get_status(room);
        if (inventory_service_get_room_type_name(svc, out_views[w].type_id, out_views[w].type_name, sizeof(out_views[w].type_name)) != SERVICE_OK) {
            snprintf(out_views[w].type_name, sizeof(out_views[w].type_name), "UNKNOWN");
        }
        ++w;
    }
    *out_count = w;
    return SERVICE_OK;
}

ServiceCode inventory_service_list_all_room_type_views(InventoryService* svc,
                                                       RoomTypeView* out_views,
                                                       size_t max_views,
                                                       size_t* out_count) {
    size_t i, n, w = 0;
    if (svc == NULL || out_views == NULL || out_count == NULL) return SERVICE_ERR_INVALID_ARG;
    n = room_type_registry_size(svc->room_type_registry);
    for (i = 0; i < n && w < max_views; ++i) {
        RoomType* rt = room_type_registry_get_at(svc->room_type_registry, i);
        if (rt == NULL) continue;
        out_views[w].type_id = room_type_get_id(rt);
        snprintf(out_views[w].type_name, sizeof(out_views[w].type_name), "%s", room_type_get_name(rt) == NULL ? "" : room_type_get_name(rt));
        ++w;
    }
    *out_count = w;
    return SERVICE_OK;
}
