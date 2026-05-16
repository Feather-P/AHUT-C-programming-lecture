#include "repository.h"

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static cJSON* guest_to_json(const Guest* guest) {
    cJSON* obj;
    if (guest == NULL) {
        return NULL;
    }
    obj = cJSON_CreateObject();
    if (obj == NULL) {
        return NULL;
    }

    if (!cJSON_AddNumberToObject(obj, "id", guest_get_id(guest)) ||
        !cJSON_AddStringToObject(obj, "prc_citizen_id", guest_get_prc_citizen_id(guest) == NULL ? "" : guest_get_prc_citizen_id(guest)) ||
        !cJSON_AddStringToObject(obj, "name", guest_get_name(guest) == NULL ? "" : guest_get_name(guest)) ||
        !cJSON_AddStringToObject(obj, "tel", guest_get_tel(guest) == NULL ? "" : guest_get_tel(guest)) ||
        !cJSON_AddNumberToObject(obj, "sex", (int)guest_get_sex(guest)) ||
        !cJSON_AddNumberToObject(obj, "checkin_count", guest_get_checkin_count(guest))) {
        cJSON_Delete(obj);
        return NULL;
    }
    return obj;
}

static int json_to_guest(const cJSON* obj, Guest** out_guest) {
    const cJSON* id = cJSON_GetObjectItemCaseSensitive(obj, "id");
    const cJSON* prc_citizen_id = cJSON_GetObjectItemCaseSensitive(obj, "prc_citizen_id");
    const cJSON* name = cJSON_GetObjectItemCaseSensitive(obj, "name");
    const cJSON* tel = cJSON_GetObjectItemCaseSensitive(obj, "tel");
    const cJSON* sex = cJSON_GetObjectItemCaseSensitive(obj, "sex");
    const cJSON* checkin_count = cJSON_GetObjectItemCaseSensitive(obj, "checkin_count");
    Guest* guest;

    if (obj == NULL || out_guest == NULL) {
        return 0;
    }
    if (!cJSON_IsNumber(id) || !cJSON_IsString(prc_citizen_id) || !cJSON_IsString(name) || !cJSON_IsString(tel) || !cJSON_IsNumber(sex)) {
        return 0;
    }

    guest = guest_create(id->valueint,
                         prc_citizen_id->valuestring,
                         name->valuestring,
                         tel->valuestring,
                         (Sex)sex->valueint,
                         cJSON_IsNumber(checkin_count) ? checkin_count->valueint : 0);
    if (guest == NULL) {
        return 0;
    }
    *out_guest = guest;
    return 1;
}

int guest_repository_save_all_to_file(const GuestRegistry* registry, const char* file_path) {
    cJSON* root;
    cJSON* guests;
    char* text;
    FILE* fp;
    size_t i;
    size_t n;

    if (registry == NULL || file_path == NULL) { return 0;
}
    root = cJSON_CreateObject();
    guests = cJSON_CreateArray();
    if (root == NULL || guests == NULL) { cJSON_Delete(root); cJSON_Delete(guests); return 0; }
    cJSON_AddItemToObject(root, "guests", guests);

    n = guest_registry_size(registry);
    for (i = 0; i < n; ++i) {
        cJSON* item = guest_to_json(guest_registry_get_at(registry, i));
        if (item == NULL) { cJSON_Delete(root); return 0; }
        cJSON_AddItemToArray(guests, item);
    }

    text = cJSON_Print(root);
    cJSON_Delete(root);
    if (text == NULL) { return 0;
}
    fp = fopen(file_path, "wb");
    if (fp == NULL) { cJSON_free(text); return 0; }
    if (fwrite(text, 1, strlen(text), fp) != strlen(text)) { fclose(fp); cJSON_free(text); return 0; }
    fclose(fp);
    cJSON_free(text);
    return 1;
}

int guest_repository_load_all_from_file(GuestRegistry* registry, const char* file_path) {
    FILE* fp;
    long fsize;
    char* text;
    size_t readn;
    cJSON* root;
    cJSON* guests;
    cJSON* item;
    if (registry == NULL || file_path == NULL) { return 0;
}

    fp = fopen(file_path, "rb");
    if (fp == NULL) { return 0;
}
    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); return 0; }
    fsize = ftell(fp);
    if (fsize < 0) { fclose(fp); return 0; }
    if (fseek(fp, 0, SEEK_SET) != 0) { fclose(fp); return 0; }
    text = (char*)malloc((size_t)fsize + 1);
    if (text == NULL) { fclose(fp); return 0; }
    readn = fread(text, 1, (size_t)fsize, fp);
    fclose(fp);
    if (readn != (size_t)fsize) { free(text); return 0; }
    text[fsize] = '\0';

    root = cJSON_Parse(text);
    free(text);
    if (root == NULL) { return 0;
}
    guests = cJSON_GetObjectItemCaseSensitive(root, "guests");
    if (!cJSON_IsArray(guests)) { cJSON_Delete(root); return 0; }

    guest_registry_clear(registry);
    cJSON_ArrayForEach(item, guests) {
        Guest* guest = NULL;
        if (!json_to_guest(item, &guest) || !guest_registry_register(registry, guest)) {
            guest_destroy(guest);
            cJSON_Delete(root);
            return 0;
        }
    }
    cJSON_Delete(root);
    return 1;
}
