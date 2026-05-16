#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "../models/room/room.h"
#include "../models/room/room_type.h"
#include "../models/price/price.h"
#include "../models/guest/guest.h"
#include "../models/order/order.h"

#include <time.h>

int room_repository_save_all_to_file(const RoomRegistry* registry, const char* file_path);
int room_repository_load_all_from_file(RoomRegistry* registry, const char* file_path);

int room_type_repository_save_all_to_file(const RoomTypeRegistry* registry, const char* file_path);
int room_type_repository_load_all_from_file(RoomTypeRegistry* registry, const char* file_path);

int guest_repository_save_all_to_file(const GuestRegistry* registry, const char* file_path);
int guest_repository_load_all_from_file(GuestRegistry* registry, const char* file_path);

int order_repository_save_all_to_file(const OrderRegistry* registry, const char* file_path);
int order_repository_load_all_from_file(OrderRegistry* registry, const char* file_path);

int price_repository_set_file_path(const char* file_path);
const char* price_repository_get_file_path(void);
int price_repository_save_all_to_file(const PriceHistory* history, const char* file_path);
int price_repository_load_all_from_file(PriceHistory* history, const char* file_path);
int price_repository_append_and_persist(PriceHistory* history,
                                        int room_type_id,
                                        double amount,
                                        time_t effective_time,
                                        const char* file_path);
int price_repository_get_current_market_price(const PriceHistory* history, int room_type_id, double* out_price);

#endif
