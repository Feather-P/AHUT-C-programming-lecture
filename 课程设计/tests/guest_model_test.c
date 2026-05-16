#include "../src/models/guest/guest.h"

#include <assert.h>
#include <stdio.h>

static void test_guest_basic_getter_setter(void) {
    Guest* g = guest_create(1, "id1", "alice", "13800000000", SEX_FEMALE, -2);
    assert(g != NULL);
    assert(guest_get_id(g) == 1);
    assert(guest_get_checkin_count(g) == 0);
    assert(guest_set_name(g, "alice2") == 1);
    assert(guest_set_tel(g, "13900000000") == 1);
    guest_set_sex(g, SEX_MALE);
    guest_increment_checkin_count(g);
    assert(guest_get_sex(g) == SEX_MALE);
    assert(guest_get_checkin_count(g) == 1);
    assert(guest_get_name(g) != NULL);
    guest_destroy(g);
}

static void test_guest_registry_ops(void) {
    GuestRegistry* r = guest_registry_create();
    Guest *a, *b;
    assert(r != NULL);
    a = guest_create(1, "id1", "a", "1", SEX_UNKNOWN, 0);
    b = guest_create(2, "id2", "b", "2", SEX_UNKNOWN, 0);
    assert(guest_registry_register(r, a) == 1);
    assert(guest_registry_register(r, b) == 1);
    assert(guest_registry_register(r, guest_create(1, "id3", "c", "3", SEX_UNKNOWN, 0)) == 0);
    assert(guest_registry_size(r) == 2);
    assert(guest_registry_find_by_prc_citizen_id(r, "id2") == b);
    assert(guest_registry_find_by_tel(r, "1") == a);
    assert(guest_registry_remove_by_id(r, 1) == 1);
    assert(guest_registry_find_by_id(r, 1) == NULL);
    guest_registry_destroy(r);
}

int main(void) {
    test_guest_basic_getter_setter();
    test_guest_registry_ops();
    puts("guest model tests passed");
    return 0;
}
