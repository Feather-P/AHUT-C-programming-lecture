#ifndef INVENTORY_SERVICE_H
#define INVENTORY_SERVICE_H

#include "service_common.h"
#include "../models/room/room.h"
#include "../models/room/room_type.h"

typedef struct InventoryService {
    RoomRegistry* room_registry;
    RoomTypeRegistry* room_type_registry;
} InventoryService;

ServiceCode inventory_service_init(InventoryService* svc,
                                   RoomRegistry* room_registry,
                                   RoomTypeRegistry* room_type_registry);
ServiceCode inventory_service_add_room(InventoryService* svc, Room* room);
ServiceCode inventory_service_update_room(InventoryService* svc, const Room* room);
ServiceCode inventory_service_remove_room(InventoryService* svc, int room_id);

ServiceCode inventory_service_mark_reserved(InventoryService* svc, int room_id);
ServiceCode inventory_service_mark_occupied(InventoryService* svc, int room_id);
ServiceCode inventory_service_mark_vacant(InventoryService* svc, int room_id);
ServiceCode inventory_service_mark_maintenance(InventoryService* svc, int room_id);

ServiceCode inventory_service_list_available_by_type(InventoryService* svc,
                                                     int room_type_id,
                                                     RoomRegistry* out_rooms);

#endif
