#ifndef INSIGHT_SERVICE_H
#define INSIGHT_SERVICE_H

#include "service_common.h"
#include "../models/room/room.h"
#include "../models/room/room_type.h"
#include "../models/guest/guest.h"
#include "../models/order/order.h"

typedef struct InsightService {
    const RoomRegistry* room_registry;
    const GuestRegistry* guest_registry;
    const OrderRegistry* order_registry;
    const RoomTypeRegistry* room_type_registry;
} InsightService;

typedef struct OccupancySnapshot {
    double overall_occupancy;
    double by_room_type[16];
    int by_room_type_count;
} OccupancySnapshot;

ServiceCode insight_service_init(InsightService* svc,
                                 const RoomRegistry* room_registry,
                                 const GuestRegistry* guest_registry,
                                 const OrderRegistry* order_registry,
                                 const RoomTypeRegistry* room_type_registry);

ServiceCode insight_service_query_room_history(InsightService* svc,
                                               int room_id,
                                               OrderRegistry* out_orders);
ServiceCode insight_service_query_guest_history(InsightService* svc,
                                                const char* id_card,
                                                OrderRegistry* out_orders);
ServiceCode insight_service_query_daily_arrivals_departures(InsightService* svc,
                                                            time_t date,
                                                            OrderRegistry* out_arrivals,
                                                            OrderRegistry* out_departures);

ServiceCode insight_service_occupancy_snapshot(InsightService* svc,
                                               time_t as_of,
                                               OccupancySnapshot* out_snapshot);
ServiceCode insight_service_top10_vip_by_month(InsightService* svc,
                                               int year,
                                               int month,
                                               GuestRegistry* out_top10);
ServiceCode insight_service_revenue_report(InsightService* svc,
                                           DateRange range,
                                           double* out_total,
                                           double by_room_type[16],
                                           int* out_type_count);
ServiceCode insight_service_longest_vacant_rooms(InsightService* svc,
                                                 time_t as_of,
                                                 RoomRegistry* out_rooms);
ServiceCode insight_service_guest_source_ratio(InsightService* svc,
                                               DateRange range,
                                               double* out_returning_ratio,
                                               double* out_new_ratio);

#endif
