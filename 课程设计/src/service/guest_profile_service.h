#ifndef GUEST_PROFILE_SERVICE_H
#define GUEST_PROFILE_SERVICE_H

#include "service_common.h"
#include "../models/guest/guest.h"

typedef struct GuestProfileService {
    GuestRegistry* guest_registry;
} GuestProfileService;

typedef struct GuestView {
    char id_card[24];
    char name[32];
    char tel[24];
    Sex sex;
    int stay_count;
} GuestView;

typedef struct CreateGuestCmd {
    char id_card[24];
    char name[32];
    char tel[24];
    Sex sex;
} CreateGuestCmd;

ServiceCode guest_profile_service_init(GuestProfileService* svc, GuestRegistry* guest_registry);
ServiceCode guest_profile_service_create_guest(GuestProfileService* svc, const CreateGuestCmd* cmd);
ServiceCode guest_profile_service_update_guest(GuestProfileService* svc, const Guest* guest);
ServiceCode guest_profile_service_remove_guest(GuestProfileService* svc, int guest_id);
ServiceCode guest_profile_service_find_by_id_card(GuestProfileService* svc,
                                                  const char* id_card,
                                                  Guest** out_guest);
ServiceCode guest_profile_service_increment_stay_count(GuestProfileService* svc,
                                                       const char* id_card);
ServiceCode guest_profile_service_list_all_guest_views(GuestProfileService* svc,
                                                       GuestView* out_views,
                                                       size_t max_views,
                                                       size_t* out_count);

#endif
