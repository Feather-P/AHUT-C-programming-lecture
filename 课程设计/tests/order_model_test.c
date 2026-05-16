#include "../src/models/order/order.h"

#include <assert.h>
#include <stdio.h>
#include <time.h>

static struct tm mk_day(int y, int m, int d) {
    struct tm t = {0};
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    return t;
}

static void test_order_lifecycle(void) {
    struct tm ci = mk_day(2026, 1, 1);
    struct tm co = mk_day(2026, 1, 2);
    Order* o = order_create(1, "id1", 101, &ci, &co, 300.0);
    assert(o != NULL);
    assert(order_get_status(o) == ORDER_STATUS_RESERVED);
    assert(order_cancel(o) == 1);
    assert(order_get_status(o) == ORDER_STATUS_CANCELLED);
    assert(order_reserve(o, 350.0) == 0);
    order_destroy(o);
}

static void test_order_registry_query(void) {
    struct tm ci = mk_day(2026, 1, 1), co = mk_day(2026, 1, 2);
    OrderRegistry* r = order_registry_create();
    Order *o1, *o2;
    assert(r != NULL);
    o1 = order_create(1, "g1", 101, &ci, &co, 100.0);
    o2 = order_create(2, "g1", 102, &ci, &co, 200.0);
    assert(order_registry_register(r, o1) == 1);
    assert(order_registry_register(r, o2) == 1);
    assert(order_registry_size(r) == 2);
    assert(order_registry_find_by_id(r, 2) == o2);
    assert(order_registry_remove_by_id(r, 1) == 1);
    assert(order_registry_find_by_id(r, 1) == NULL);
    order_registry_destroy(r);
}

int main(void) {
    test_order_lifecycle();
    test_order_registry_query();
    puts("order model tests passed");
    return 0;
}

