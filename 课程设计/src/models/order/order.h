#ifndef ORDER_H_
#define ORDER_H_

#include <stddef.h>
#include <time.h>

#include "../price/price.h"

enum OrderStatus;

typedef struct Order Order;
typedef struct OrderRegistry OrderRegistry;
typedef enum OrderStatus OrderStatus;

enum OrderStatus {
    ORDER_STATUS_RESERVED = 0,
    ORDER_STATUS_CANCELLED = 1,
    ORDER_STATUS_CHECKED_IN = 2,
    ORDER_STATUS_CHECKED_OUT = 3
};

Order* order_create(
    int id,
    const char* guest_prc_citizen_id,
    int room_id,
    const struct tm* schedule_checkin_date,
    const struct tm* schedule_checkout_date,
    double market_price);
void order_destroy(Order* order);

int order_get_id(const Order* order);
void order_set_id(Order* order, int id);

const char* order_get_guest_prc_citizen_id(const Order* order);
int order_set_guest_prc_citizen_id(Order* order, const char* guest_prc_citizen_id);

int order_get_room_id(const Order* order);
void order_set_room_id(Order* order, int room_id);

const struct tm* order_get_schedule_checkin_date(const Order* order);
void order_set_schedule_checkin_date(Order* order, const struct tm* schedule_checkin_date);

const struct tm* order_get_schedule_checkout_date(const Order* order);
void order_set_schedule_checkout_date(Order* order, const struct tm* schedule_checkout_date);

time_t order_get_actual_checkin_time(const Order* order);
void order_set_actual_checkin_time(Order* order, time_t actual_checkin_time);

time_t order_get_actual_checkout_time(const Order* order);
void order_set_actual_checkout_time(Order* order, time_t actual_checkout_time);

double order_get_cost(const Order* order);
void order_set_cost(Order* order, double cost);

OrderStatus order_get_status(const Order* order);

int order_reserve(Order* order, double market_price);
int order_cancel(Order* order);
int order_extend_stay(Order* order, const struct tm* new_schedule_checkout_date, double market_price);
int order_checkin(Order* order, time_t when);
int order_checkout(Order* order, time_t when);
int order_set_status_for_restore(Order* order, OrderStatus status);

int order_reserve_with_market_price(Order* order, const PriceHistory* history, int room_type_id);
int order_extend_stay_with_market_price(Order* order,
                                        const struct tm* new_schedule_checkout_date,
                                        const PriceHistory* history,
                                        int room_type_id);

OrderRegistry* order_registry_create(void);
void order_registry_destroy(OrderRegistry* registry);
void order_registry_clear(OrderRegistry* registry);

int order_registry_register(OrderRegistry* registry, Order* order);
Order* order_registry_find_by_id(const OrderRegistry* registry, int id);
int order_registry_remove_by_id(OrderRegistry* registry, int id);

size_t order_registry_size(const OrderRegistry* registry);
Order* order_registry_get_at(const OrderRegistry* registry, size_t index);

size_t order_registry_query_by_guest_id(const OrderRegistry* registry,
                                        const char* guest_prc_citizen_id,
                                        Order** out,
                                        size_t cap);
size_t order_registry_query_by_status(const OrderRegistry* registry,
                                      OrderStatus status,
                                      Order** out,
                                      size_t cap);
size_t order_registry_query_by_actual_checkout_range(const OrderRegistry* registry,
                                                     time_t begin_inclusive,
                                                     time_t end_inclusive,
                                                     Order** out,
                                                     size_t cap);

#endif
