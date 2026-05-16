#include "guest.h"

#include "../../utils/linked_list.h"

#include <stdlib.h>
#include <string.h>

struct Guest {
    int id;
    char* prc_citizen_id;
    char* name;
    char* tel;
    Sex sex;
    int checkin_count;
};

struct GuestRegistry {
    LinkedList* guests;
};

static char* guest_strdup(const char* src) {
    if (src == NULL) {
        return NULL;
    }

    size_t len = strlen(src);
    char* dst = (char*)malloc(len + 1);
    if (dst == NULL) {
        return NULL;
    }

    memcpy(dst, src, len + 1);
    return dst;
}

Guest* guest_create(int id,
                    const char* prc_citizen_id,
                    const char* name,
                    const char* tel,
                    Sex sex,
                    int checkin_count) {
    Guest* guest = (Guest*)malloc(sizeof(Guest));
    if (guest == NULL) {
        return NULL;
    }

    guest->id = id;
    guest->sex = sex;
    guest->checkin_count = checkin_count < 0 ? 0 : checkin_count;

    guest->prc_citizen_id = guest_strdup(prc_citizen_id);
    if (prc_citizen_id != NULL && guest->prc_citizen_id == NULL) {
        free(guest);
        return NULL;
    }

    guest->name = guest_strdup(name);
    if (name != NULL && guest->name == NULL) {
        free(guest->prc_citizen_id);
        free(guest);
        return NULL;
    }

    guest->tel = guest_strdup(tel);
    if (tel != NULL && guest->tel == NULL) {
        free(guest->name);
        free(guest->prc_citizen_id);
        free(guest);
        return NULL;
    }

    return guest;
}

void guest_destroy(Guest* guest) {
    if (guest == NULL) {
        return;
    }

    free(guest->prc_citizen_id);
    free(guest->name);
    free(guest->tel);
    free(guest);
}

int guest_get_id(const Guest* guest) {
    if (guest == NULL) {
        return -1;
    }
    return guest->id;
}

void guest_set_id(Guest* guest, int id) {
    if (guest == NULL) {
        return;
    }
    guest->id = id;
}

const char* guest_get_prc_citizen_id(const Guest* guest) {
    if (guest == NULL) {
        return NULL;
    }
    return guest->prc_citizen_id;
}

int guest_set_prc_citizen_id(Guest* guest, const char* prc_citizen_id) {
    char* new_prc_citizen_id;
    if (guest == NULL) {
        return 0;
    }

    new_prc_citizen_id = guest_strdup(prc_citizen_id);
    if (prc_citizen_id != NULL && new_prc_citizen_id == NULL) {
        return 0;
    }

    free(guest->prc_citizen_id);
    guest->prc_citizen_id = new_prc_citizen_id;
    return 1;
}

const char* guest_get_name(const Guest* guest) {
    if (guest == NULL) {
        return NULL;
    }
    return guest->name;
}

int guest_set_name(Guest* guest, const char* name) {
    char* new_name;
    if (guest == NULL) {
        return 0;
    }

    new_name = guest_strdup(name);
    if (name != NULL && new_name == NULL) {
        return 0;
    }

    free(guest->name);
    guest->name = new_name;
    return 1;
}

const char* guest_get_tel(const Guest* guest) {
    if (guest == NULL) {
        return NULL;
    }
    return guest->tel;
}

int guest_set_tel(Guest* guest, const char* tel) {
    char* new_tel;
    if (guest == NULL) {
        return 0;
    }

    new_tel = guest_strdup(tel);
    if (tel != NULL && new_tel == NULL) {
        return 0;
    }

    free(guest->tel);
    guest->tel = new_tel;
    return 1;
}

Sex guest_get_sex(const Guest* guest) {
    if (guest == NULL) {
        return SEX_UNKNOWN;
    }
    return guest->sex;
}

void guest_set_sex(Guest* guest, Sex sex) {
    if (guest == NULL) {
        return;
    }
    guest->sex = sex;
}

int guest_get_checkin_count(const Guest* guest) {
    if (guest == NULL) return 0;
    return guest->checkin_count;
}

void guest_set_checkin_count(Guest* guest, int checkin_count) {
    if (guest == NULL) return;
    guest->checkin_count = checkin_count < 0 ? 0 : checkin_count;
}

void guest_increment_checkin_count(Guest* guest) {
    if (guest == NULL) return;
    guest->checkin_count += 1;
}

GuestRegistry* guest_registry_create(void) {
    GuestRegistry* registry = (GuestRegistry*)malloc(sizeof(GuestRegistry));
    if (registry == NULL) {
        return NULL;
    }

    registry->guests = linked_list_create((linked_list_free_fn)guest_destroy);
    if (registry->guests == NULL) {
        free(registry);
        return NULL;
    }

    return registry;
}

void guest_registry_clear(GuestRegistry* registry) {
    if (registry == NULL || registry->guests == NULL) {
        return;
    }

    linked_list_clear(registry->guests);
}

void guest_registry_destroy(GuestRegistry* registry) {
    if (registry == NULL) {
        return;
    }

    linked_list_destroy(registry->guests);
    free(registry);
}

Guest* guest_registry_find_by_id(const GuestRegistry* registry, int id) {
    size_t i;
    size_t n;

    if (registry == NULL || registry->guests == NULL) {
        return NULL;
    }

    n = linked_list_size(registry->guests);
    for (i = 0; i < n; ++i) {
        Guest* guest = (Guest*)linked_list_get(registry->guests, i);
        if (guest != NULL && guest_get_id(guest) == id) {
            return guest;
        }
    }

    return NULL;
}

Guest* guest_registry_find_by_prc_citizen_id(const GuestRegistry* registry, const char* prc_citizen_id) {
    size_t i;
    size_t n;

    if (registry == NULL || registry->guests == NULL || prc_citizen_id == NULL) {
        return NULL;
    }

    n = linked_list_size(registry->guests);
    for (i = 0; i < n; ++i) {
        Guest* guest = (Guest*)linked_list_get(registry->guests, i);
        const char* guest_prc_citizen_id;
        if (guest == NULL) {
            continue;
        }

        guest_prc_citizen_id = guest_get_prc_citizen_id(guest);
        if (guest_prc_citizen_id != NULL && strcmp(guest_prc_citizen_id, prc_citizen_id) == 0) {
            return guest;
        }
    }

    return NULL;
}

Guest* guest_registry_find_by_tel(const GuestRegistry* registry, const char* tel) {
    size_t i;
    size_t n;

    if (registry == NULL || registry->guests == NULL || tel == NULL) {
        return NULL;
    }

    n = linked_list_size(registry->guests);
    for (i = 0; i < n; ++i) {
        Guest* guest = (Guest*)linked_list_get(registry->guests, i);
        const char* guest_tel;
        if (guest == NULL) {
            continue;
        }

        guest_tel = guest_get_tel(guest);
        if (guest_tel != NULL && strcmp(guest_tel, tel) == 0) {
            return guest;
        }
    }

    return NULL;
}

int guest_registry_register(GuestRegistry* registry, Guest* guest) {
    if (registry == NULL || registry->guests == NULL || guest == NULL) {
        return 0;
    }

    if (guest_registry_find_by_id(registry, guest_get_id(guest)) != NULL) {
        return 0;
    }

    return linked_list_push_back(registry->guests, guest);
}

int guest_registry_remove_by_id(GuestRegistry* registry, int id) {
    size_t i;
    size_t n;

    if (registry == NULL || registry->guests == NULL) {
        return 0;
    }

    n = linked_list_size(registry->guests);
    for (i = 0; i < n; ++i) {
        Guest* guest = (Guest*)linked_list_get(registry->guests, i);
        if (guest != NULL && guest_get_id(guest) == id) {
            Guest* removed = (Guest*)linked_list_remove_at(registry->guests, i);
            guest_destroy(removed);
            return 1;
        }
    }

    return 0;
}

size_t guest_registry_size(const GuestRegistry* registry) {
    if (registry == NULL || registry->guests == NULL) {
        return 0;
    }
    return linked_list_size(registry->guests);
}

Guest* guest_registry_get_at(const GuestRegistry* registry, size_t index) {
    if (registry == NULL || registry->guests == NULL) {
        return NULL;
    }
    return (Guest*)linked_list_get(registry->guests, index);
}
