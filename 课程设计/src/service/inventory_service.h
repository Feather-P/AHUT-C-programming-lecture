#ifndef INVENTORY_SERVICE_H
#define INVENTORY_SERVICE_H

#include "service_common.h"
#include "../models/room/room.h"
#include "../models/room/room_type.h"

typedef struct InventoryService {
    RoomRegistry* room_registry;
    RoomTypeRegistry* room_type_registry;
} InventoryService;

typedef struct RoomView {
    char room_no[32];
    int floor;
    int bed_count;
    double area;
    int type_id;
    char type_name[32];
    RoomStatus status;
} RoomView;

typedef struct RoomTypeView {
    int type_id;
    char type_name[32];
} RoomTypeView;

typedef struct CreateRoomCmd {
    char room_no[32];
    int floor;
    int bed_count;
    double area;
    char description[64];
    int type_id;
} CreateRoomCmd;

ServiceCode inventory_service_init(InventoryService* svc,
                                   RoomRegistry* room_registry,
                                   RoomTypeRegistry* room_type_registry);
ServiceCode inventory_service_create_room(InventoryService* svc, const CreateRoomCmd* cmd);
ServiceCode inventory_service_update_room(InventoryService* svc, const Room* room);
ServiceCode inventory_service_remove_room(InventoryService* svc, int room_id);

ServiceCode inventory_service_mark_reserved(InventoryService* svc, int room_id);
ServiceCode inventory_service_mark_occupied(InventoryService* svc, int room_id);
ServiceCode inventory_service_mark_vacant(InventoryService* svc, int room_id);
ServiceCode inventory_service_mark_maintenance(InventoryService* svc, int room_id);

ServiceCode inventory_service_list_available_by_type(InventoryService* svc,
                                                     int room_type_id,
                                                     RoomRegistry* out_rooms);
ServiceCode inventory_service_list_all_rooms(InventoryService* svc,
                                             RoomRegistry* out_rooms);

ServiceCode inventory_service_add_room_type(InventoryService* svc, RoomType* room_type);
ServiceCode inventory_service_update_room_type_name(InventoryService* svc, int room_type_id, const char* new_name);
ServiceCode inventory_service_remove_room_type(InventoryService* svc, int room_type_id);
ServiceCode inventory_service_get_room_type_name(InventoryService* svc, int room_type_id, char* out_name, size_t out_name_size);

ServiceCode inventory_service_list_all_room_views(InventoryService* svc,
                                                  RoomView* out_views,
                                                  size_t max_views,
                                                  size_t* out_count);
ServiceCode inventory_service_list_all_room_type_views(InventoryService* svc,
                                                       RoomTypeView* out_views,
                                                       size_t max_views,
                                                       size_t* out_count);

#endif
