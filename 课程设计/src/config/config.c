#include "config.h"

#include <libconfig.h>

static const char* g_room_file_path = "data/rooms.json";
static const char* g_room_type_file_path = "data/room_types.json";
static const char* g_guest_file_path = "data/guests.json";
static const char* g_order_file_path = "data/orders.json";
static const char* g_price_file_path = "data/prices.json";
static const char* g_config_file_path = "config/repository.cfg";

static int is_valid_path(const char* file_path) {
    return file_path != 0 && file_path[0] != '\0';
}

const char* config_get_config_file_path(void) { return g_config_file_path; }

int config_load(void) {
    config_t cfg;
    const char* value = NULL;

    config_init(&cfg);
    if (!config_read_file(&cfg, g_config_file_path)) {
        config_destroy(&cfg);
        return 0;
    }

    if (config_lookup_string(&cfg, "repository.room_file_path", &value) && is_valid_path(value)) {
        g_room_file_path = value;
    }
    if (config_lookup_string(&cfg, "repository.room_type_file_path", &value) && is_valid_path(value)) {
        g_room_type_file_path = value;
    }
    if (config_lookup_string(&cfg, "repository.guest_file_path", &value) && is_valid_path(value)) {
        g_guest_file_path = value;
    }
    if (config_lookup_string(&cfg, "repository.order_file_path", &value) && is_valid_path(value)) {
        g_order_file_path = value;
    }
    if (config_lookup_string(&cfg, "repository.price_file_path", &value) && is_valid_path(value)) {
        g_price_file_path = value;
    }

    config_destroy(&cfg);
    return 1;
}

const char* config_get_room_file_path(void) { return g_room_file_path; }
const char* config_get_room_type_file_path(void) { return g_room_type_file_path; }
const char* config_get_guest_file_path(void) { return g_guest_file_path; }
const char* config_get_order_file_path(void) { return g_order_file_path; }
const char* config_get_price_file_path(void) { return g_price_file_path; }

void config_get_repository_config(RepositoryConfig* out_config) {
    if (out_config == NULL) {
        return;
    }
    out_config->room_file_path = g_room_file_path;
    out_config->room_type_file_path = g_room_type_file_path;
    out_config->guest_file_path = g_guest_file_path;
    out_config->order_file_path = g_order_file_path;
    out_config->price_file_path = g_price_file_path;
}
