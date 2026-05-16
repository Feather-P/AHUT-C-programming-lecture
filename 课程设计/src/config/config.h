#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

typedef struct RepositoryConfig {
    const char* room_file_path;
    const char* room_type_file_path;
    const char* guest_file_path;
    const char* order_file_path;
    const char* price_file_path;
} RepositoryConfig;

int config_load(void);
const char* config_get_config_file_path(void);

const char* config_get_room_file_path(void);
const char* config_get_room_type_file_path(void);
const char* config_get_guest_file_path(void);
const char* config_get_order_file_path(void);
const char* config_get_price_file_path(void);

void config_get_repository_config(RepositoryConfig* out_config);

#endif
