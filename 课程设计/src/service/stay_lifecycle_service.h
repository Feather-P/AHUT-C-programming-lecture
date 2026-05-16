#ifndef STAY_LIFECYCLE_SERVICE_H
#define STAY_LIFECYCLE_SERVICE_H

#include "service_common.h"
#include "inventory_service.h"
#include "guest_profile_service.h"
#include "../models/order/order.h"
#include "../models/price/price.h"

typedef struct StayLifecycleService {
    OrderRegistry* order_registry;
    PriceHistory* price_history;
    InventoryService* inventory_service;
    GuestProfileService* guest_service;
} StayLifecycleService;

typedef struct CreateReservationCmd {
    int order_id;
    char guest_id_card[24];
    int room_id;
    time_t reserved_at;
    time_t checkin_date;
    time_t checkout_date;
} CreateReservationCmd;

typedef struct SettlementResult {
    int nights;
    double unit_price;
    double total_amount;
} SettlementResult;

ServiceCode stay_service_init(StayLifecycleService* svc,
                              OrderRegistry* order_registry,
                              PriceHistory* price_history,
                              InventoryService* inventory_service,
                              GuestProfileService* guest_service);

ServiceCode stay_service_create_reservation(StayLifecycleService* svc,
                                            const CreateReservationCmd* cmd);
ServiceCode stay_service_cancel_reservation(StayLifecycleService* svc,
                                            int order_id);
ServiceCode stay_service_check_in(StayLifecycleService* svc, int order_id, time_t checkin_at);
ServiceCode stay_service_extend_checkout_date(StayLifecycleService* svc,
                                              int order_id,
                                              time_t new_checkout_date);
ServiceCode stay_service_change_room(StayLifecycleService* svc,
                                     int order_id,
                                     int new_room_id);
ServiceCode stay_service_check_out_and_settle(StayLifecycleService* svc,
                                              int order_id,
                                              time_t checkout_at,
                                              SettlementResult* out_settlement);

ServiceCode stay_service_calculate_nights(time_t checkin_date,
                                          time_t checkout_date,
                                          int* out_nights);
ServiceCode stay_service_calculate_amount(StayLifecycleService* svc,
                                          int room_type_id,
                                          int nights,
                                          double* out_amount,
                                          double* out_unit_price);

#endif
