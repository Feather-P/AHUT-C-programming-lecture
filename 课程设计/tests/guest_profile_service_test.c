#include "../src/service/guest_profile_service.h"

#include <assert.h>
#include <stdio.h>

const char* config_get_guest_file_path(void) { return "./guest_test.json"; }
int guest_repository_save_all_to_file(const GuestRegistry* registry, const char* file_path) {
    (void)registry;
    (void)file_path;
    return 1;
}

int main(void) {
    GuestRegistry* gr = guest_registry_create();
    GuestProfileService svc;
    Guest* g;
    assert(gr != NULL);
    assert(guest_profile_service_init(&svc, gr) == SERVICE_OK);
    g = guest_create(1, "id1", "n", "1", SEX_UNKNOWN, 0);
    assert(g != NULL);
    assert(guest_profile_service_add_guest(&svc, g) == SERVICE_OK);
    assert(guest_profile_service_add_guest(&svc, guest_create(1, "id2", "m", "2", SEX_UNKNOWN, 0)) == SERVICE_ERR_CONFLICT);
    assert(guest_profile_service_increment_stay_count(&svc, "id1") == SERVICE_OK);
    guest_registry_destroy(gr);
    puts("guest profile service tests passed");
    return 0;
}
