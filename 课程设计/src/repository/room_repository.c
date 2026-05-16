#include "repository.h"

#include "../models/room/room.h"
#include "../models/room/room_type.h"

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static cJSON* room_to_json(const Room* room) {
    cJSON* obj;
    if (room == NULL) {
        return NULL;
    }

    obj = cJSON_CreateObject();
    if (obj == NULL) {
        return NULL;
    }

    if (!cJSON_AddNumberToObject(obj, "room_id", room_get_id(room)) ||
        !cJSON_AddStringToObject(obj, "room_no", room_get_no(room) == NULL ? "" : room_get_no(room)) ||
        !cJSON_AddNumberToObject(obj, "floor", room_get_floor(room)) ||
        !cJSON_AddNumberToObject(obj, "bed_count", room_get_bed_count(room)) ||
        !cJSON_AddNumberToObject(obj, "area", room_get_area(room)) ||
        !cJSON_AddStringToObject(obj, "description", room_get_description(room) == NULL ? "" : room_get_description(room)) ||
        !cJSON_AddNumberToObject(obj, "type_id", room_get_type_id(room)) ||
        !cJSON_AddNumberToObject(obj, "status", (int)room_get_status(room)) ||
        !cJSON_AddNumberToObject(obj, "created_at", (double)room_get_created_at(room)) ||
        !cJSON_AddNumberToObject(obj, "updated_at", (double)room_get_updated_at(room)) ||
        !cJSON_AddNumberToObject(obj, "last_checkout_time", (double)room_get_last_checkout_time(room))) {
        cJSON_Delete(obj);
        return NULL;
    }

    return obj;
}

static int json_to_room(const cJSON* obj, Room** out_room) {
    const cJSON* room_id;
    const cJSON* room_no;
    const cJSON* floor;
    const cJSON* bed_count;
    const cJSON* area;
    const cJSON* description;
    const cJSON* type_id;
    const cJSON* status;
    const cJSON* created_at;
    const cJSON* updated_at;
    const cJSON* last_checkout_time;
    Room* room;

    if (obj == NULL || out_room == NULL) {
        return 0;
    }

    room_id = cJSON_GetObjectItemCaseSensitive(obj, "room_id");
    room_no = cJSON_GetObjectItemCaseSensitive(obj, "room_no");
    floor = cJSON_GetObjectItemCaseSensitive(obj, "floor");
    bed_count = cJSON_GetObjectItemCaseSensitive(obj, "bed_count");
    area = cJSON_GetObjectItemCaseSensitive(obj, "area");
    description = cJSON_GetObjectItemCaseSensitive(obj, "description");
    type_id = cJSON_GetObjectItemCaseSensitive(obj, "type_id");
    status = cJSON_GetObjectItemCaseSensitive(obj, "status");
    created_at = cJSON_GetObjectItemCaseSensitive(obj, "created_at");
    updated_at = cJSON_GetObjectItemCaseSensitive(obj, "updated_at");
    last_checkout_time = cJSON_GetObjectItemCaseSensitive(obj, "last_checkout_time");

    if (!cJSON_IsNumber(room_id) || !cJSON_IsString(room_no) || !cJSON_IsNumber(floor) || !cJSON_IsNumber(bed_count) ||
        !cJSON_IsNumber(area) || !cJSON_IsString(description) || !cJSON_IsNumber(type_id) ||
        !cJSON_IsNumber(status) || !cJSON_IsNumber(created_at) || !cJSON_IsNumber(updated_at) ||
        !cJSON_IsNumber(last_checkout_time)) {
        return 0;
    }

    room = room_create(room_id->valueint,
                       room_no->valuestring,
                       floor->valueint,
                       bed_count->valueint,
                       area->valuedouble,
                       description->valuestring,
                       type_id->valueint,
                       (RoomStatus)status->valueint,
                       (time_t)created_at->valuedouble,
                       (time_t)updated_at->valuedouble,
                       (time_t)last_checkout_time->valuedouble);
    if (room == NULL) {
        return 0;
    }

    *out_room = room;
    return 1;
}

int room_repository_save_all_to_file(const RoomRegistry* registry, const char* file_path) {
    size_t i;
    size_t n;
    cJSON* root;
    cJSON* rooms;
    char* text;
    FILE* fp;

    if (registry == NULL || file_path == NULL) {
        return 0;
    }

    root = cJSON_CreateObject();
    rooms = cJSON_CreateArray();
    if (root == NULL || rooms == NULL) {
        cJSON_Delete(root);
        cJSON_Delete(rooms);
        return 0;
    }
    cJSON_AddItemToObject(root, "rooms", rooms);

    n = room_registry_size(registry);
    for (i = 0; i < n; ++i) {
        Room* room = room_registry_get_at(registry, i);
        cJSON* item = room_to_json(room);
        if (item == NULL) {
            cJSON_Delete(root);
            return 0;
        }
        cJSON_AddItemToArray(rooms, item);
    }

    text = cJSON_Print(root);
    cJSON_Delete(root);
    if (text == NULL) {
        return 0;
    }

    fp = fopen(file_path, "wb");
    if (fp == NULL) {
        cJSON_free(text);
        return 0;
    }

    if (fwrite(text, 1, strlen(text), fp) != strlen(text)) {
        fclose(fp);
        cJSON_free(text);
        return 0;
    }

    fclose(fp);
    cJSON_free(text);
    return 1;
}

int room_repository_load_all_from_file(RoomRegistry* registry, const char* file_path) {
    FILE* fp;
    long fsize;
    char* text;
    size_t readn;
    cJSON* root;
    cJSON* rooms;
    cJSON* item;

    if (registry == NULL || file_path == NULL) {
        return 0;
    }

    fp = fopen(file_path, "rb");
    if (fp == NULL) {
        return 0;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return 0;
    }
    fsize = ftell(fp);
    if (fsize < 0) {
        fclose(fp);
        return 0;
    }
    if (fseek(fp, 0, SEEK_SET) != 0) {
        fclose(fp);
        return 0;
    }

    text = (char*)malloc((size_t)fsize + 1);
    if (text == NULL) {
        fclose(fp);
        return 0;
    }

    readn = fread(text, 1, (size_t)fsize, fp);
    fclose(fp);
    if (readn != (size_t)fsize) {
        free(text);
        return 0;
    }
    text[fsize] = '\0';

    root = cJSON_Parse(text);
    free(text);
    if (root == NULL) {
        return 0;
    }

    rooms = cJSON_GetObjectItemCaseSensitive(root, "rooms");
    if (!cJSON_IsArray(rooms)) {
        cJSON_Delete(root);
        return 0;
    }

    room_registry_clear(registry);
    cJSON_ArrayForEach(item, rooms) {
        Room* room = NULL;
        if (!json_to_room(item, &room) || !room_registry_register(registry, room)) {
            room_destroy(room);
            cJSON_Delete(root);
            return 0;
        }
    }

    cJSON_Delete(root);
    return 1;
}

static cJSON* room_type_to_json(const RoomType* room_type) {
    cJSON* obj;
    if (room_type == NULL) {
        return NULL;
    }

    obj = cJSON_CreateObject();
    if (obj == NULL) {
        return NULL;
    }

    if (!cJSON_AddNumberToObject(obj, "id", room_type_get_id(room_type)) ||
        !cJSON_AddStringToObject(obj, "type_name", room_type_get_name(room_type) == NULL ? "" : room_type_get_name(room_type))) {
        cJSON_Delete(obj);
        return NULL;
    }

    return obj;
}

static int json_to_room_type(const cJSON* obj, RoomType** out_room_type) {
    const cJSON* id;
    const cJSON* type_name;
    RoomType* room_type;

    if (obj == NULL || out_room_type == NULL) {
        return 0;
    }

    id = cJSON_GetObjectItemCaseSensitive(obj, "id");
    type_name = cJSON_GetObjectItemCaseSensitive(obj, "type_name");
    if (!cJSON_IsNumber(id) || !cJSON_IsString(type_name)) {
        return 0;
    }

    room_type = room_type_create(id->valueint, type_name->valuestring);
    if (room_type == NULL) {
        return 0;
    }

    *out_room_type = room_type;
    return 1;
}

int room_type_repository_save_all_to_file(const RoomTypeRegistry* registry, const char* file_path) {
    cJSON* root;
    cJSON* room_types;
    char* text;
    FILE* fp;
    size_t i;
    size_t n;

    if (registry == NULL || file_path == NULL) {
        return 0;
    }

    root = cJSON_CreateObject();
    room_types = cJSON_CreateArray();
    if (root == NULL || room_types == NULL) {
        cJSON_Delete(root);
        cJSON_Delete(room_types);
        return 0;
    }
    cJSON_AddItemToObject(root, "room_types", room_types);

    n = room_type_registry_size(registry);
    for (i = 0; i < n; ++i) {
        RoomType* room_type = room_type_registry_get_at(registry, i);
        cJSON* item = room_type_to_json(room_type);
        if (item == NULL) {
            cJSON_Delete(root);
            return 0;
        }
        cJSON_AddItemToArray(room_types, item);
    }

    text = cJSON_Print(root);
    cJSON_Delete(root);
    if (text == NULL) {
        return 0;
    }

    fp = fopen(file_path, "wb");
    if (fp == NULL) {
        cJSON_free(text);
        return 0;
    }

    if (fwrite(text, 1, strlen(text), fp) != strlen(text)) {
        fclose(fp);
        cJSON_free(text);
        return 0;
    }

    fclose(fp);
    cJSON_free(text);
    return 1;
}

int room_type_repository_load_all_from_file(RoomTypeRegistry* registry, const char* file_path) {
    FILE* fp;
    long fsize;
    char* text;
    size_t readn;
    cJSON* root;
    cJSON* room_types;
    cJSON* item;

    if (registry == NULL || file_path == NULL) {
        return 0;
    }

    fp = fopen(file_path, "rb");
    if (fp == NULL) {
        return 0;
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
    if (root == NULL) { return 0; }

    room_types = cJSON_GetObjectItemCaseSensitive(root, "room_types");
    if (!cJSON_IsArray(room_types)) { cJSON_Delete(root); return 0; }

    room_type_registry_clear(registry);
    cJSON_ArrayForEach(item, room_types) {
        RoomType* room_type = NULL;
        if (!json_to_room_type(item, &room_type) || !room_type_registry_register(registry, room_type)) {
            room_type_destroy(room_type);
            cJSON_Delete(root);
            return 0;
        }
    }

    cJSON_Delete(root);
    return 1;
}
