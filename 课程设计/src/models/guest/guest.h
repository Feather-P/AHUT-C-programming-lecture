#ifndef GUEST_H_
#define GUEST_H_

#include <stddef.h>

enum Sex;

typedef struct Guest Guest;
typedef struct GuestRegistry GuestRegistry;
typedef enum Sex Sex;

enum Sex {
    SEX_UNKNOWN = 0,
    SEX_MALE = 1,
    SEX_FEMALE = 2
};

Guest* guest_create(int id,
                    const char* prc_citizen_id,
                    const char* name,
                    const char* tel,
                    Sex sex,
                    int checkin_count);
void guest_destroy(Guest* guest);

int guest_get_id(const Guest* guest);
void guest_set_id(Guest* guest, int id);

const char* guest_get_prc_citizen_id(const Guest* guest);
int guest_set_prc_citizen_id(Guest* guest, const char* prc_citizen_id);

const char* guest_get_name(const Guest* guest);
int guest_set_name(Guest* guest, const char* name);

const char* guest_get_tel(const Guest* guest);
int guest_set_tel(Guest* guest, const char* tel);

Sex guest_get_sex(const Guest* guest);
void guest_set_sex(Guest* guest, Sex sex);

int guest_get_checkin_count(const Guest* guest);
void guest_set_checkin_count(Guest* guest, int checkin_count);
void guest_increment_checkin_count(Guest* guest);

GuestRegistry* guest_registry_create(void);
void guest_registry_destroy(GuestRegistry* registry);
void guest_registry_clear(GuestRegistry* registry);

int guest_registry_register(GuestRegistry* registry, Guest* guest);
Guest* guest_registry_find_by_id(const GuestRegistry* registry, int id);
Guest* guest_registry_find_by_prc_citizen_id(const GuestRegistry* registry, const char* prc_citizen_id);
Guest* guest_registry_find_by_tel(const GuestRegistry* registry, const char* tel);
int guest_registry_remove_by_id(GuestRegistry* registry, int id);

size_t guest_registry_size(const GuestRegistry* registry);
Guest* guest_registry_get_at(const GuestRegistry* registry, size_t index);



#endif
