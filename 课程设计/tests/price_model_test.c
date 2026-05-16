#include "../src/models/price/price.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    PriceHistory* h = price_history_create();
    Price *p1, *p2;
    assert(h != NULL);
    p1 = price_create(1, 100.0, 10);
    p2 = price_create(1, 120.0, 20);
    assert(p1 && p2);
    assert(price_history_append(h, p1) == 1);
    assert(price_history_append(h, p2) == 1);
    assert(price_history_size(h) == 2);
    assert(price_history_get_latest_by_room_type_id(h, 1) == p2);
    price_history_destroy(h);
    puts("price model tests passed");
    return 0;
}
