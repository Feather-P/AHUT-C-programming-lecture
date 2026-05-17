#include "../src/service/guest_profile_service.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

const char* config_get_guest_file_path(void) { return "./guest_test.json"; }
int guest_repository_save_all_to_file(const GuestRegistry* registry, const char* file_path) {
    (void)registry;
    (void)file_path;
    return 1;
}

int main(void) {
    GuestRegistry* gr = guest_registry_create();
    GuestProfileService svc;
    CreateGuestCmd c1;
    CreateGuestCmd c2;
    assert(gr != NULL);
    assert(guest_profile_service_init(&svc, gr) == SERVICE_OK);
    memset(&c1, 0, sizeof(c1));
    snprintf(c1.id_card, sizeof(c1.id_card), "%s", "id1");
    snprintf(c1.name, sizeof(c1.name), "%s", "n");
    snprintf(c1.tel, sizeof(c1.tel), "%s", "1");
    c1.sex = SEX_UNKNOWN;
    assert(guest_profile_service_create_guest(&svc, &c1) == SERVICE_OK);
    memset(&c2, 0, sizeof(c2));
    snprintf(c2.id_card, sizeof(c2.id_card), "%s", "id1");
    snprintf(c2.name, sizeof(c2.name), "%s", "m");
    snprintf(c2.tel, sizeof(c2.tel), "%s", "2");
    c2.sex = SEX_UNKNOWN;
    assert(guest_profile_service_create_guest(&svc, &c2) == SERVICE_ERR_CONFLICT);
    assert(guest_profile_service_increment_stay_count(&svc, "id1") == SERVICE_OK);
    guest_registry_destroy(gr);
    puts("guest profile service tests passed");
    return 0;
}
