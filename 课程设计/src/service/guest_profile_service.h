#ifndef GUEST_PROFILE_SERVICE_H
#define GUEST_PROFILE_SERVICE_H

#include "service_common.h"
#include "../models/guest/guest.h"

typedef struct GuestProfileService {
    GuestRegistry* guest_registry;
} GuestProfileService;

ServiceCode guest_profile_service_init(GuestProfileService* svc, GuestRegistry* guest_registry);
ServiceCode guest_profile_service_add_guest(GuestProfileService* svc, Guest* guest);
ServiceCode guest_profile_service_update_guest(GuestProfileService* svc, const Guest* guest);
ServiceCode guest_profile_service_remove_guest(GuestProfileService* svc, int guest_id);
ServiceCode guest_profile_service_find_by_id_card(GuestProfileService* svc,
                                                  const char* id_card,
                                                  Guest** out_guest);
ServiceCode guest_profile_service_increment_stay_count(GuestProfileService* svc,
                                                       const char* id_card);

#endif
