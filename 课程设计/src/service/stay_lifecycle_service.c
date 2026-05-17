#include "stay_lifecycle_service.h"

#include "../config/config.h"
#include "../repository/repository.h"
#include "../utils/logger.h"

#include <string.h>

static int next_order_id(OrderRegistry* orr) {
    size_t i, n;
    int max_id = 0;
    n = order_registry_size(orr);
    for (i = 0; i < n; ++i) {
        Order* o = order_registry_get_at(orr, i);
        if (o != NULL && order_get_id(o) > max_id) max_id = order_get_id(o);
    }
    return max_id + 1;
}

static ServiceCode to_tm(time_t t, struct tm* out_tm) {
    struct tm* p;
    if (out_tm == NULL || t <= (time_t)0) return SERVICE_ERR_INVALID_ARG;
    p = localtime(&t);
    if (p == NULL) return SERVICE_ERR_INTERNAL;
    *out_tm = *p;
    out_tm->tm_hour = 0;
    out_tm->tm_min = 0;
    out_tm->tm_sec = 0;
    return SERVICE_OK;
}

ServiceCode stay_service_init(StayLifecycleService* svc,
                              OrderRegistry* order_registry,
                              PriceHistory* price_history,
                              InventoryService* inventory_service,
                              GuestProfileService* guest_service) {
    if (svc == NULL || order_registry == NULL || price_history == NULL || inventory_service == NULL || guest_service == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    svc->order_registry = order_registry;
    svc->price_history = price_history;
    svc->inventory_service = inventory_service;
    svc->guest_service = guest_service;
    return SERVICE_OK;
}

ServiceCode stay_service_calculate_nights(time_t checkin_date, time_t checkout_date, int* out_nights) {
    double days;
    if (out_nights == NULL || checkout_date <= checkin_date) return SERVICE_ERR_INVALID_ARG;
    days = difftime(checkout_date, checkin_date) / 86400.0;
    if (days <= 0) return SERVICE_ERR_INVALID_ARG;
    *out_nights = (int)days;
    return SERVICE_OK;
}

ServiceCode stay_service_calculate_amount(StayLifecycleService* svc,
                                          int room_type_id,
                                          int nights,
                                          double* out_amount,
                                          double* out_unit_price) {
    double unit_price;
    if (svc == NULL || svc->price_history == NULL || nights <= 0 || out_amount == NULL || out_unit_price == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    if (!price_repository_get_current_market_price(svc->price_history, room_type_id, &unit_price)) {
        return SERVICE_ERR_NOT_FOUND;
    }
    *out_unit_price = unit_price;
    *out_amount = unit_price * nights;
    return SERVICE_OK;
}

ServiceCode stay_service_create_reservation(StayLifecycleService* svc, const CreateReservationCmd* cmd) {
    Room* room;
    struct tm ci, co;
    int nights;
    double amount, unit_price;
    Order* order;
    ServiceCode rc;
    int order_id;
    if (svc == NULL || cmd == NULL || cmd->guest_id_card[0] == '\0') return SERVICE_ERR_INVALID_ARG;
    room = room_registry_find_by_id(svc->inventory_service->room_registry, cmd->room_id);
    if (room == NULL) return SERVICE_ERR_NOT_FOUND;
    if (room_get_status(room) != ROOM_STATUS_IDLE) return SERVICE_ERR_CONFLICT;
    rc = stay_service_calculate_nights(cmd->checkin_date, cmd->checkout_date, &nights);
    if (rc != SERVICE_OK) return rc;
    rc = stay_service_calculate_amount(svc, room_get_type_id(room), nights, &amount, &unit_price);
    if (rc != SERVICE_OK) return rc;
    if (to_tm(cmd->checkin_date, &ci) != SERVICE_OK || to_tm(cmd->checkout_date, &co) != SERVICE_OK) return SERVICE_ERR_INVALID_ARG;
    order_id = next_order_id(svc->order_registry);
    order = order_create(order_id, cmd->guest_id_card, cmd->room_id, &ci, &co, amount);
    if (order == NULL) return SERVICE_ERR_INTERNAL;
    if (!order_registry_register(svc->order_registry, order)) { order_destroy(order); return SERVICE_ERR_INTERNAL; }
    rc = inventory_service_mark_reserved(svc->inventory_service, cmd->room_id);
    if (rc != SERVICE_OK) return rc;
    LOG_INFO("stay_service_create_reservation: persist orders to '%s'", config_get_order_file_path());
    if (!order_repository_save_all_to_file(svc->order_registry, config_get_order_file_path())) {
        LOG_ERROR("stay_service_create_reservation: persist failed path='%s'", config_get_order_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    return SERVICE_OK;
}

ServiceCode stay_service_cancel_reservation(StayLifecycleService* svc, int order_id) {
    Order* order;
    ServiceCode rc;
    if (svc == NULL) return SERVICE_ERR_INVALID_ARG;
    order = order_registry_find_by_id(svc->order_registry, order_id);
    if (order == NULL) return SERVICE_ERR_NOT_FOUND;
    if (!order_cancel(order)) return SERVICE_ERR_INVALID_STATE;
    rc = inventory_service_mark_vacant(svc->inventory_service, order_get_room_id(order));
    if (rc == SERVICE_OK) {
        LOG_INFO("stay_service_cancel_reservation: persist orders to '%s'", config_get_order_file_path());
        if (!order_repository_save_all_to_file(svc->order_registry, config_get_order_file_path())) {
            LOG_ERROR("stay_service_cancel_reservation: persist failed path='%s'", config_get_order_file_path());
            return SERVICE_ERR_INTERNAL;
        }
    }
    return rc;
}

ServiceCode stay_service_check_in(StayLifecycleService* svc, int order_id, time_t checkin_at) {
    Order* order;
    ServiceCode rc;
    if (svc == NULL || checkin_at <= (time_t)0) return SERVICE_ERR_INVALID_ARG;
    order = order_registry_find_by_id(svc->order_registry, order_id);
    if (order == NULL) return SERVICE_ERR_NOT_FOUND;
    if (!order_checkin(order, checkin_at)) return SERVICE_ERR_INVALID_STATE;
    rc = inventory_service_mark_occupied(svc->inventory_service, order_get_room_id(order));
    if (rc == SERVICE_OK) {
        guest_profile_service_increment_stay_count(svc->guest_service, order_get_guest_prc_citizen_id(order));
        LOG_INFO("stay_service_check_in: persist orders to '%s'", config_get_order_file_path());
        if (!order_repository_save_all_to_file(svc->order_registry, config_get_order_file_path())) {
            LOG_ERROR("stay_service_check_in: persist failed path='%s'", config_get_order_file_path());
            return SERVICE_ERR_INTERNAL;
        }
    }
    return rc;
}

ServiceCode stay_service_extend_checkout_date(StayLifecycleService* svc, int order_id, time_t new_checkout_date) {
    Order* order;
    int nights;
    double amount, unit_price;
    struct tm co;
    ServiceCode rc;
    Room* room;
    if (svc == NULL) return SERVICE_ERR_INVALID_ARG;
    order = order_registry_find_by_id(svc->order_registry, order_id);
    if (order == NULL) return SERVICE_ERR_NOT_FOUND;
    rc = stay_service_calculate_nights(mktime((struct tm*)order_get_schedule_checkin_date(order)), new_checkout_date, &nights);
    if (rc != SERVICE_OK) return rc;
    room = room_registry_find_by_id(svc->inventory_service->room_registry, order_get_room_id(order));
    if (room == NULL) return SERVICE_ERR_NOT_FOUND;
    rc = stay_service_calculate_amount(svc, room_get_type_id(room), nights, &amount, &unit_price);
    if (rc != SERVICE_OK) return rc;
    if (to_tm(new_checkout_date, &co) != SERVICE_OK) return SERVICE_ERR_INVALID_ARG;
    if (!order_extend_stay(order, &co, amount)) return SERVICE_ERR_INVALID_STATE;
    LOG_INFO("stay_service_extend_checkout_date: persist orders to '%s'", config_get_order_file_path());
    if (!order_repository_save_all_to_file(svc->order_registry, config_get_order_file_path())) {
        LOG_ERROR("stay_service_extend_checkout_date: persist failed path='%s'", config_get_order_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    return SERVICE_OK;
}

ServiceCode stay_service_change_room(StayLifecycleService* svc, int order_id, int new_room_id) {
    Order* order;
    ServiceCode rc;
    if (svc == NULL) return SERVICE_ERR_INVALID_ARG;
    order = order_registry_find_by_id(svc->order_registry, order_id);
    if (order == NULL) return SERVICE_ERR_NOT_FOUND;
    if (order_get_status(order) != ORDER_STATUS_CHECKED_IN) return SERVICE_ERR_INVALID_STATE;
    rc = inventory_service_mark_reserved(svc->inventory_service, new_room_id);
    if (rc != SERVICE_OK) return rc;
    rc = inventory_service_mark_vacant(svc->inventory_service, order_get_room_id(order));
    if (rc != SERVICE_OK) return rc;
    order_set_room_id(order, new_room_id);
    inventory_service_mark_occupied(svc->inventory_service, new_room_id);
    return SERVICE_OK;
}

ServiceCode stay_service_check_out_and_settle(StayLifecycleService* svc,
                                              int order_id,
                                              time_t checkout_at,
                                              SettlementResult* out_settlement) {
    Order* order;
    int nights;
    double unit_price = 0;
    ServiceCode rc;
    Room* room;
    if (svc == NULL || out_settlement == NULL || checkout_at <= (time_t)0) return SERVICE_ERR_INVALID_ARG;
    order = order_registry_find_by_id(svc->order_registry, order_id);
    if (order == NULL) return SERVICE_ERR_NOT_FOUND;
    if (!order_checkout(order, checkout_at)) return SERVICE_ERR_INVALID_STATE;
    rc = inventory_service_mark_vacant(svc->inventory_service, order_get_room_id(order));
    if (rc != SERVICE_OK) return rc;
    rc = stay_service_calculate_nights(order_get_actual_checkin_time(order), checkout_at, &nights);
    if (rc != SERVICE_OK) return rc;
    room = room_registry_find_by_id(svc->inventory_service->room_registry, order_get_room_id(order));
    if (room != NULL) {
        double amount = order_get_cost(order);
        if (nights > 0) unit_price = amount / nights;
        out_settlement->nights = nights;
        out_settlement->unit_price = unit_price;
        out_settlement->total_amount = amount;
    }
    LOG_INFO("stay_service_check_out_and_settle: persist orders to '%s'", config_get_order_file_path());
    if (!order_repository_save_all_to_file(svc->order_registry, config_get_order_file_path())) {
        LOG_ERROR("stay_service_check_out_and_settle: persist failed path='%s'", config_get_order_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    return SERVICE_OK;
}

ServiceCode stay_service_list_order_views(StayLifecycleService* svc,
                                          StayOrderView* out_views,
                                          size_t max_views,
                                          size_t* out_count) {
    size_t i;
    size_t n;
    if (svc == NULL || svc->order_registry == NULL || out_views == NULL || out_count == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    n = order_registry_size(svc->order_registry);
    if (n > max_views) n = max_views;
    for (i = 0; i < n; ++i) {
        Order* o = order_registry_get_at(svc->order_registry, i);
        if (o == NULL) continue;
        out_views[i].order_id = order_get_id(o);
        strncpy(out_views[i].guest_id_card, order_get_guest_prc_citizen_id(o), sizeof(out_views[i].guest_id_card) - 1);
        out_views[i].guest_id_card[sizeof(out_views[i].guest_id_card) - 1] = '\0';
        out_views[i].room_id = order_get_room_id(o);
        out_views[i].status = order_get_status(o);
        out_views[i].checkin_time = order_get_actual_checkin_time(o);
        out_views[i].checkout_time = order_get_actual_checkout_time(o);
    }
    *out_count = n;
    return SERVICE_OK;
}

ServiceCode stay_service_list_order_views_by_status(StayLifecycleService* svc,
                                                    OrderStatus status,
                                                    StayOrderView* out_views,
                                                    size_t max_views,
                                                    size_t* out_count) {
    size_t i;
    size_t count = 0;
    size_t n;
    if (svc == NULL || svc->order_registry == NULL || out_views == NULL || out_count == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    n = order_registry_size(svc->order_registry);
    for (i = 0; i < n && count < max_views; ++i) {
        Order* o = order_registry_get_at(svc->order_registry, i);
        if (o == NULL || order_get_status(o) != status) continue;
        out_views[count].order_id = order_get_id(o);
        strncpy(out_views[count].guest_id_card, order_get_guest_prc_citizen_id(o), sizeof(out_views[count].guest_id_card) - 1);
        out_views[count].guest_id_card[sizeof(out_views[count].guest_id_card) - 1] = '\0';
        out_views[count].room_id = order_get_room_id(o);
        out_views[count].status = order_get_status(o);
        out_views[count].checkin_time = order_get_actual_checkin_time(o);
        out_views[count].checkout_time = order_get_actual_checkout_time(o);
        ++count;
    }
    *out_count = count;
    return SERVICE_OK;
}
