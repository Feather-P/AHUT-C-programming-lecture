#include "../src/service/stay_lifecycle_service.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

const char* config_get_order_file_path(void) { return "./order_test.json"; }
const char* config_get_room_file_path(void) { return "./room_test.json"; }
const char* config_get_guest_file_path(void) { return "./guest_test.json"; }
const char* config_get_room_type_file_path(void) { return "./room_type_test.json"; }

int main(void) {
    OrderRegistry* or = order_registry_create();
    PriceHistory* ph = price_history_create();
    RoomRegistry* rr = room_registry_create();
    RoomTypeRegistry* tr = room_type_registry_create();
    GuestRegistry* gr = guest_registry_create();
    InventoryService inv;
    GuestProfileService gps;
    StayLifecycleService svc;
    int nights = 0;
    double amount = 0.0, unit = 0.0;

    assert(or && ph && rr && tr && gr);
    assert(guest_profile_service_init(&gps, gr) == SERVICE_OK);
    assert(inventory_service_init(&inv, rr, tr) == SERVICE_OK);
    assert(stay_service_init(&svc, or, ph, &inv, &gps) == SERVICE_OK);

    assert(stay_service_calculate_nights(100, 100 + 86400 * 2, &nights) == SERVICE_OK);
    assert(nights == 2);

    assert(price_history_append(ph, price_create(1, 200.0, time(NULL))) == 1);
    assert(stay_service_calculate_amount(&svc, 1, 3, &amount, &unit) == SERVICE_OK);
    assert(unit == 200.0);
    assert(amount == 600.0);

    order_registry_destroy(or);
    price_history_destroy(ph);
    room_registry_destroy(rr);
    room_type_registry_destroy(tr);
    guest_registry_destroy(gr);

    puts("stay lifecycle service tests passed");
    return 0;
}
