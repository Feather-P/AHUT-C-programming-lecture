#include "insight_service.h"

ServiceCode insight_service_init(InsightService* svc,
                                 const RoomRegistry* room_registry,
                                 const GuestRegistry* guest_registry,
                                 const OrderRegistry* order_registry,
                                 const RoomTypeRegistry* room_type_registry) {
    if (svc == NULL || room_registry == NULL || guest_registry == NULL || order_registry == NULL || room_type_registry == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    svc->room_registry = room_registry;
    svc->guest_registry = guest_registry;
    svc->order_registry = order_registry;
    svc->room_type_registry = room_type_registry;
    return SERVICE_OK;
}

ServiceCode insight_service_query_room_history(InsightService* svc, int room_id, OrderRegistry* out_orders) {
    (void)svc;
    (void)room_id;
    (void)out_orders;
    return SERVICE_OK;
}

ServiceCode insight_service_query_guest_history(InsightService* svc, const char* id_card, OrderRegistry* out_orders) {
    (void)svc;
    (void)id_card;
    (void)out_orders;
    return SERVICE_OK;
}

ServiceCode insight_service_query_daily_arrivals_departures(InsightService* svc,
                                                            time_t date,
                                                            OrderRegistry* out_arrivals,
                                                            OrderRegistry* out_departures) {
    (void)svc;
    (void)date;
    (void)out_arrivals;
    (void)out_departures;
    return SERVICE_OK;
}

ServiceCode insight_service_occupancy_snapshot(InsightService* svc, time_t as_of, OccupancySnapshot* out_snapshot) {
    (void)svc;
    (void)as_of;
    if (out_snapshot == NULL) return SERVICE_ERR_INVALID_ARG;
    out_snapshot->overall_occupancy = 0.0;
    out_snapshot->by_room_type_count = 0;
    return SERVICE_OK;
}

ServiceCode insight_service_top10_vip_by_month(InsightService* svc, int year, int month, GuestRegistry* out_top10) {
    (void)svc;
    (void)year;
    (void)month;
    (void)out_top10;
    return SERVICE_OK;
}

ServiceCode insight_service_revenue_report(InsightService* svc,
                                           DateRange range,
                                           double* out_total,
                                           double by_room_type[16],
                                           int* out_type_count) {
    (void)svc;
    (void)range;
    (void)by_room_type;
    if (out_total == NULL || out_type_count == NULL) return SERVICE_ERR_INVALID_ARG;
    *out_total = 0.0;
    *out_type_count = 0;
    return SERVICE_OK;
}

ServiceCode insight_service_longest_vacant_rooms(InsightService* svc, time_t as_of, RoomRegistry* out_rooms) {
    (void)svc;
    (void)as_of;
    (void)out_rooms;
    return SERVICE_OK;
}

ServiceCode insight_service_guest_source_ratio(InsightService* svc,
                                               DateRange range,
                                               double* out_returning_ratio,
                                               double* out_new_ratio) {
    (void)svc;
    (void)range;
    if (out_returning_ratio == NULL || out_new_ratio == NULL) return SERVICE_ERR_INVALID_ARG;
    *out_returning_ratio = 0.0;
    *out_new_ratio = 0.0;
    return SERVICE_OK;
}

