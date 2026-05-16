#include "guest_profile_service.h"

#include "../config/config.h"
#include "../repository/repository.h"
#include "../utils/logger.h"

ServiceCode guest_profile_service_init(GuestProfileService* svc, GuestRegistry* guest_registry) {
    if (svc == NULL || guest_registry == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    svc->guest_registry = guest_registry;
    return SERVICE_OK;
}

ServiceCode guest_profile_service_add_guest(GuestProfileService* svc, Guest* guest) {
    if (svc == NULL || svc->guest_registry == NULL || guest == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    if (guest_registry_find_by_id(svc->guest_registry, guest_get_id(guest)) != NULL ||
        guest_registry_find_by_prc_citizen_id(svc->guest_registry, guest_get_prc_citizen_id(guest)) != NULL) {
        return SERVICE_ERR_CONFLICT;
    }
    if (!guest_registry_register(svc->guest_registry, guest)) {
        return SERVICE_ERR_INTERNAL;
    }
    LOG_INFO("guest_profile_service_add_guest: persist guests to '%s'", config_get_guest_file_path());
    if (!guest_repository_save_all_to_file(svc->guest_registry, config_get_guest_file_path())) {
        LOG_ERROR("guest_profile_service_add_guest: persist failed path='%s'", config_get_guest_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    return SERVICE_OK;
}

ServiceCode guest_profile_service_update_guest(GuestProfileService* svc, const Guest* guest) {
    Guest* target;
    if (svc == NULL || svc->guest_registry == NULL || guest == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    target = guest_registry_find_by_id(svc->guest_registry, guest_get_id(guest));
    if (target == NULL) {
        return SERVICE_ERR_NOT_FOUND;
    }
    if (guest_get_name(guest) != NULL && !guest_set_name(target, guest_get_name(guest))) return SERVICE_ERR_INTERNAL;
    if (guest_get_tel(guest) != NULL && !guest_set_tel(target, guest_get_tel(guest))) return SERVICE_ERR_INTERNAL;
    guest_set_sex(target, guest_get_sex(guest));
    guest_set_checkin_count(target, guest_get_checkin_count(guest));
    LOG_INFO("guest_profile_service_update_guest: persist guests to '%s'", config_get_guest_file_path());
    if (!guest_repository_save_all_to_file(svc->guest_registry, config_get_guest_file_path())) {
        LOG_ERROR("guest_profile_service_update_guest: persist failed path='%s'", config_get_guest_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    return SERVICE_OK;
}

ServiceCode guest_profile_service_remove_guest(GuestProfileService* svc, int guest_id) {
    if (svc == NULL || svc->guest_registry == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    if (!guest_registry_remove_by_id(svc->guest_registry, guest_id)) {
        return SERVICE_ERR_NOT_FOUND;
    }
    LOG_INFO("guest_profile_service_remove_guest: persist guests to '%s'", config_get_guest_file_path());
    if (!guest_repository_save_all_to_file(svc->guest_registry, config_get_guest_file_path())) {
        LOG_ERROR("guest_profile_service_remove_guest: persist failed path='%s'", config_get_guest_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    return SERVICE_OK;
}

ServiceCode guest_profile_service_find_by_id_card(GuestProfileService* svc,
                                                  const char* id_card,
                                                  Guest** out_guest) {
    Guest* g;
    if (svc == NULL || svc->guest_registry == NULL || id_card == NULL || out_guest == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    g = guest_registry_find_by_prc_citizen_id(svc->guest_registry, id_card);
    if (g == NULL) {
        return SERVICE_ERR_NOT_FOUND;
    }
    *out_guest = g;
    return SERVICE_OK;
}

ServiceCode guest_profile_service_increment_stay_count(GuestProfileService* svc, const char* id_card) {
    Guest* g;
    if (svc == NULL || svc->guest_registry == NULL || id_card == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    g = guest_registry_find_by_prc_citizen_id(svc->guest_registry, id_card);
    if (g == NULL) {
        return SERVICE_ERR_NOT_FOUND;
    }
    guest_increment_checkin_count(g);
    LOG_INFO("guest_profile_service_increment_stay_count: persist guests to '%s'", config_get_guest_file_path());
    if (!guest_repository_save_all_to_file(svc->guest_registry, config_get_guest_file_path())) {
        LOG_ERROR("guest_profile_service_increment_stay_count: persist failed path='%s'", config_get_guest_file_path());
        return SERVICE_ERR_INTERNAL;
    }
    return SERVICE_OK;
}
