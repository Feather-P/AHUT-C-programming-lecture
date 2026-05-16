#include "order.h"

#include "../../utils/linked_list.h"

#include <stdlib.h>
#include <string.h>

struct Order {
    int id;
    char* guest_prc_citizen_id;
    int room_id;
    struct tm schedule_checkin_date;
    struct tm schedule_checkout_date;
    time_t actual_checkin_time;
    time_t actual_checkout_time;
    double cost;
    OrderStatus status;
};

struct OrderRegistry {
    LinkedList* orders;
};

static char* order_strdup(const char* src) {
    if (src == NULL) {
        return NULL;
    }

    size_t len = strlen(src);
    char* dst = (char*)malloc(len + 1);
    if (dst == NULL) {
        return NULL;
    }

    memcpy(dst, src, len + 1);
    return dst;
}

Order* order_create(int id,
                    const char* guest_prc_citizen_id,
                    int room_id,
                    const struct tm* schedule_checkin_date,
                    const struct tm* schedule_checkout_date,
                    double market_price) {
    Order* order = (Order*)malloc(sizeof(Order));
    if (order == NULL) {
        return NULL;
    }

    order->id = id;
    order->room_id = room_id;
    order->schedule_checkin_date = schedule_checkin_date ? *schedule_checkin_date : (struct tm){0};
    order->schedule_checkout_date = schedule_checkout_date ? *schedule_checkout_date : (struct tm){0};
    order->actual_checkin_time = (time_t)0;
    order->actual_checkout_time = (time_t)0;
    order->cost = market_price;
    order->status = ORDER_STATUS_RESERVED;

    order->guest_prc_citizen_id = order_strdup(guest_prc_citizen_id);
    if (guest_prc_citizen_id != NULL && order->guest_prc_citizen_id == NULL) {
        free(order);
        return NULL;
    }

    return order;
}

void order_destroy(Order* order) {
    if (order == NULL) {
        return;
    }

    free(order->guest_prc_citizen_id);
    free(order);
}

int order_get_id(const Order* order) { return order ? order->id : -1; }
void order_set_id(Order* order, int id) { if (order) { order->id = id; 
}}

const char* order_get_guest_prc_citizen_id(const Order* order) { return order ? order->guest_prc_citizen_id : NULL; }
int order_set_guest_prc_citizen_id(Order* order, const char* guest_prc_citizen_id) {
    char* new_id;
    if (order == NULL) {
        return 0;
    }

    new_id = order_strdup(guest_prc_citizen_id);
    if (guest_prc_citizen_id != NULL && new_id == NULL) {
        return 0;
    }

    free(order->guest_prc_citizen_id);
    order->guest_prc_citizen_id = new_id;
    return 1;
}

int order_get_room_id(const Order* order) { return order ? order->room_id : -1; }
void order_set_room_id(Order* order, int room_id) { if (order) { order->room_id = room_id; 
}}

const struct tm* order_get_schedule_checkin_date(const Order* order) { return order ? &order->schedule_checkin_date : NULL; }
void order_set_schedule_checkin_date(Order* order, const struct tm* schedule_checkin_date) {
    if (order == NULL || schedule_checkin_date == NULL) {
        return;
    }
    order->schedule_checkin_date = *schedule_checkin_date;
}

const struct tm* order_get_schedule_checkout_date(const Order* order) { return order ? &order->schedule_checkout_date : NULL; }
void order_set_schedule_checkout_date(Order* order, const struct tm* schedule_checkout_date) {
    if (order == NULL || schedule_checkout_date == NULL) {
        return;
    }
    order->schedule_checkout_date = *schedule_checkout_date;
}

time_t order_get_actual_checkin_time(const Order* order) { return order ? order->actual_checkin_time : (time_t)0; }
void order_set_actual_checkin_time(Order* order, time_t actual_checkin_time) { if (order) { order->actual_checkin_time = actual_checkin_time; 
}}

time_t order_get_actual_checkout_time(const Order* order) { return order ? order->actual_checkout_time : (time_t)0; }
void order_set_actual_checkout_time(Order* order, time_t actual_checkout_time) { if (order) { order->actual_checkout_time = actual_checkout_time; 
}}

double order_get_cost(const Order* order) { return order ? order->cost : 0.0; }
void order_set_cost(Order* order, double cost) { if (order) { order->cost = cost; 
}}

OrderStatus order_get_status(const Order* order) { return order ? order->status : ORDER_STATUS_CANCELLED; }

int order_reserve(Order* order, double market_price) {
    if (order == NULL || market_price < 0.0) {
        return 0;
    }

    if (order->status == ORDER_STATUS_CANCELLED || order->status == ORDER_STATUS_CHECKED_OUT) {
        return 0;
    }

    order->cost = market_price;
    order->status = ORDER_STATUS_RESERVED;
    return 1;
}

int order_cancel(Order* order) {
    if (order == NULL) {
        return 0;
    }

    if (order->status != ORDER_STATUS_RESERVED) {
        return 0;
    }

    order->status = ORDER_STATUS_CANCELLED;
    return 1;
}

int order_extend_stay(Order* order, const struct tm* new_schedule_checkout_date, double market_price) {
    if (order == NULL || new_schedule_checkout_date == NULL || market_price < 0.0) {
        return 0;
    }

    if (order->status != ORDER_STATUS_RESERVED && order->status != ORDER_STATUS_CHECKED_IN) {
        return 0;
    }

    order->schedule_checkout_date = *new_schedule_checkout_date;
    order->cost = market_price;
    return 1;
}

int order_checkin(Order* order, time_t when) {
    if (order == NULL || when <= (time_t)0) {
        return 0;
    }
    if (order->status != ORDER_STATUS_RESERVED) {
        return 0;
    }
    order->actual_checkin_time = when;
    order->status = ORDER_STATUS_CHECKED_IN;
    return 1;
}

int order_checkout(Order* order, time_t when) {
    if (order == NULL || when <= (time_t)0) {
        return 0;
    }
    if (order->status != ORDER_STATUS_CHECKED_IN) {
        return 0;
    }
    if (order->actual_checkin_time != (time_t)0 && when < order->actual_checkin_time) {
        return 0;
    }
    order->actual_checkout_time = when;
    order->status = ORDER_STATUS_CHECKED_OUT;
    return 1;
}

int order_set_status_for_restore(Order* order, OrderStatus status) {
    if (order == NULL) {
        return 0;
    }
    if (status < ORDER_STATUS_RESERVED || status > ORDER_STATUS_CHECKED_OUT) {
        return 0;
    }
    order->status = status;
    return 1;
}

OrderRegistry* order_registry_create(void) {
    OrderRegistry* registry = (OrderRegistry*)malloc(sizeof(OrderRegistry));
    if (registry == NULL) {
        return NULL;
    }

    registry->orders = linked_list_create((linked_list_free_fn)order_destroy);
    if (registry->orders == NULL) {
        free(registry);
        return NULL;
    }

    return registry;
}

void order_registry_destroy(OrderRegistry* registry) {
    if (registry == NULL) {
        return;
    }
    linked_list_destroy(registry->orders);
    free(registry);
}

void order_registry_clear(OrderRegistry* registry) {
    if (registry == NULL || registry->orders == NULL) {
        return;
    }
    linked_list_clear(registry->orders);
}

int order_registry_register(OrderRegistry* registry, Order* order) {
    if (registry == NULL || registry->orders == NULL || order == NULL) {
        return 0;
    }

    if (order_registry_find_by_id(registry, order_get_id(order)) != NULL) {
        return 0;
    }

    return linked_list_push_back(registry->orders, order);
}

Order* order_registry_find_by_id(const OrderRegistry* registry, int id) {
    size_t i;
    size_t n;

    if (registry == NULL || registry->orders == NULL) {
        return NULL;
    }

    n = linked_list_size(registry->orders);
    for (i = 0; i < n; ++i) {
        Order* order = (Order*)linked_list_get(registry->orders, i);
        if (order != NULL && order_get_id(order) == id) {
            return order;
        }
    }
    return NULL;
}

int order_registry_remove_by_id(OrderRegistry* registry, int id) {
    size_t i;
    size_t n;

    if (registry == NULL || registry->orders == NULL) {
        return 0;
    }

    n = linked_list_size(registry->orders);
    for (i = 0; i < n; ++i) {
        Order* order = (Order*)linked_list_get(registry->orders, i);
        if (order != NULL && order_get_id(order) == id) {
            Order* removed = (Order*)linked_list_remove_at(registry->orders, i);
            order_destroy(removed);
            return 1;
        }
    }

    return 0;
}

size_t order_registry_size(const OrderRegistry* registry) {
    if (registry == NULL || registry->orders == NULL) {
        return 0;
    }
    return linked_list_size(registry->orders);
}

Order* order_registry_get_at(const OrderRegistry* registry, size_t index) {
    if (registry == NULL || registry->orders == NULL) {
        return NULL;
    }
    return (Order*)linked_list_get(registry->orders, index);
}

size_t order_registry_query_by_guest_id(const OrderRegistry* registry,
                                        const char* guest_prc_citizen_id,
                                        Order** out,
                                        size_t cap) {
    size_t i;
    size_t n;
    size_t count = 0;

    if (registry == NULL || registry->orders == NULL || guest_prc_citizen_id == NULL) {
        return 0;
    }

    n = linked_list_size(registry->orders);
    for (i = 0; i < n; ++i) {
        Order* order = (Order*)linked_list_get(registry->orders, i);
        const char* gid;
        if (order == NULL) {
            continue;
        }
        gid = order_get_guest_prc_citizen_id(order);
        if (gid != NULL && strcmp(gid, guest_prc_citizen_id) == 0) {
            if (out != NULL && count < cap) {
                out[count] = order;
            }
            ++count;
        }
    }
    return count;
}

size_t order_registry_query_by_status(const OrderRegistry* registry,
                                      OrderStatus status,
                                      Order** out,
                                      size_t cap) {
    size_t i;
    size_t n;
    size_t count = 0;

    if (registry == NULL || registry->orders == NULL) {
        return 0;
    }

    n = linked_list_size(registry->orders);
    for (i = 0; i < n; ++i) {
        Order* order = (Order*)linked_list_get(registry->orders, i);
        if (order != NULL && order_get_status(order) == status) {
            if (out != NULL && count < cap) {
                out[count] = order;
            }
            ++count;
        }
    }
    return count;
}

size_t order_registry_query_by_actual_checkout_range(const OrderRegistry* registry,
                                                     time_t begin_inclusive,
                                                     time_t end_inclusive,
                                                     Order** out,
                                                     size_t cap) {
    size_t i;
    size_t n;
    size_t count = 0;

    if (registry == NULL || registry->orders == NULL || begin_inclusive > end_inclusive) {
        return 0;
    }

    n = linked_list_size(registry->orders);
    for (i = 0; i < n; ++i) {
        Order* order = (Order*)linked_list_get(registry->orders, i);
        time_t t;
        if (order == NULL) {
            continue;
        }
        t = order_get_actual_checkout_time(order);
        if (t >= begin_inclusive && t <= end_inclusive) {
            if (out != NULL && count < cap) {
                out[count] = order;
            }
            ++count;
        }
    }
    return count;
}
