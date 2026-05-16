#include "repository.h"

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static cJSON* tm_to_json(const struct tm* t) {
    cJSON* obj;
    if (t == NULL) { return NULL;}
    obj = cJSON_CreateObject();
    if (obj == NULL) { return NULL;
}
    if (!cJSON_AddNumberToObject(obj, "year", t->tm_year) ||
        !cJSON_AddNumberToObject(obj, "mon", t->tm_mon) ||
        !cJSON_AddNumberToObject(obj, "mday", t->tm_mday) ||
        !cJSON_AddNumberToObject(obj, "hour", t->tm_hour) ||
        !cJSON_AddNumberToObject(obj, "min", t->tm_min) ||
        !cJSON_AddNumberToObject(obj, "sec", t->tm_sec)) {
        cJSON_Delete(obj);
        return NULL;
    }
    return obj;
}

static int json_to_tm(const cJSON* obj, struct tm* out) {
    const cJSON* year;
    const cJSON* mon;
    const cJSON* mday;
    const cJSON* hour;
    const cJSON* min;
    const cJSON* sec;
    if (obj == NULL || out == NULL) { return 0;
}
    year = cJSON_GetObjectItemCaseSensitive(obj, "year");
    mon = cJSON_GetObjectItemCaseSensitive(obj, "mon");
    mday = cJSON_GetObjectItemCaseSensitive(obj, "mday");
    hour = cJSON_GetObjectItemCaseSensitive(obj, "hour");
    min = cJSON_GetObjectItemCaseSensitive(obj, "min");
    sec = cJSON_GetObjectItemCaseSensitive(obj, "sec");
    if (!cJSON_IsNumber(year) || !cJSON_IsNumber(mon) || !cJSON_IsNumber(mday) ||
        !cJSON_IsNumber(hour) || !cJSON_IsNumber(min) || !cJSON_IsNumber(sec)) {
        return 0;
    }
    memset(out, 0, sizeof(*out));
    out->tm_year = year->valueint;
    out->tm_mon = mon->valueint;
    out->tm_mday = mday->valueint;
    out->tm_hour = hour->valueint;
    out->tm_min = min->valueint;
    out->tm_sec = sec->valueint;
    return 1;
}

static cJSON* order_to_json(const Order* order) {
    cJSON* obj;
    cJSON* checkin;
    cJSON* checkout;
    if (order == NULL) { return NULL;
}
    obj = cJSON_CreateObject();
    if (obj == NULL) { return NULL;
}
    checkin = tm_to_json(order_get_schedule_checkin_date(order));
    checkout = tm_to_json(order_get_schedule_checkout_date(order));
    if (checkin == NULL || checkout == NULL ||
        !cJSON_AddNumberToObject(obj, "id", order_get_id(order)) ||
        !cJSON_AddStringToObject(obj, "guest_prc_citizen_id", order_get_guest_prc_citizen_id(order) == NULL ? "" : order_get_guest_prc_citizen_id(order)) ||
        !cJSON_AddNumberToObject(obj, "room_id", order_get_room_id(order)) ||
        !cJSON_AddItemToObject(obj, "schedule_checkin_date", checkin) ||
        !cJSON_AddItemToObject(obj, "schedule_checkout_date", checkout) ||
        !cJSON_AddNumberToObject(obj, "actual_checkin_time", (double)order_get_actual_checkin_time(order)) ||
        !cJSON_AddNumberToObject(obj, "actual_checkout_time", (double)order_get_actual_checkout_time(order)) ||
        !cJSON_AddNumberToObject(obj, "cost", order_get_cost(order)) ||
        !cJSON_AddNumberToObject(obj, "status", (int)order_get_status(order))) {
        cJSON_Delete(checkin);
        cJSON_Delete(checkout);
        cJSON_Delete(obj);
        return NULL;
    }
    return obj;
}

static int json_to_order(const cJSON* obj, Order** out_order) {
    const cJSON* id;
    const cJSON* guest_id;
    const cJSON* room_id;
    const cJSON* schedule_checkin_date;
    const cJSON* schedule_checkout_date;
    const cJSON* actual_checkin_time;
    const cJSON* actual_checkout_time;
    const cJSON* cost;
    const cJSON* status;
    struct tm checkin_date;
    struct tm checkout_date;
    Order* order;
    if (obj == NULL || out_order == NULL) { return 0;
}
    id = cJSON_GetObjectItemCaseSensitive(obj, "id");
    guest_id = cJSON_GetObjectItemCaseSensitive(obj, "guest_prc_citizen_id");
    room_id = cJSON_GetObjectItemCaseSensitive(obj, "room_id");
    schedule_checkin_date = cJSON_GetObjectItemCaseSensitive(obj, "schedule_checkin_date");
    schedule_checkout_date = cJSON_GetObjectItemCaseSensitive(obj, "schedule_checkout_date");
    actual_checkin_time = cJSON_GetObjectItemCaseSensitive(obj, "actual_checkin_time");
    actual_checkout_time = cJSON_GetObjectItemCaseSensitive(obj, "actual_checkout_time");
    cost = cJSON_GetObjectItemCaseSensitive(obj, "cost");
    status = cJSON_GetObjectItemCaseSensitive(obj, "status");
    if (!cJSON_IsNumber(id) || !cJSON_IsString(guest_id) || !cJSON_IsNumber(room_id) ||
        !cJSON_IsObject(schedule_checkin_date) || !cJSON_IsObject(schedule_checkout_date) ||
        !cJSON_IsNumber(actual_checkin_time) || !cJSON_IsNumber(actual_checkout_time) || !cJSON_IsNumber(cost) ||
        !cJSON_IsNumber(status)) {
        return 0;
    }
    if (!json_to_tm(schedule_checkin_date, &checkin_date) || !json_to_tm(schedule_checkout_date, &checkout_date)) { return 0;
}
    order = order_create(id->valueint, guest_id->valuestring, room_id->valueint, &checkin_date, &checkout_date, cost->valuedouble);
    if (order == NULL) { return 0;
}
    order_set_actual_checkin_time(order, (time_t)actual_checkin_time->valuedouble);
    order_set_actual_checkout_time(order, (time_t)actual_checkout_time->valuedouble);
    if (!order_set_status_for_restore(order, (OrderStatus)status->valueint)) {
        order_destroy(order);
        return 0;
    }
    *out_order = order;
    return 1;
}

int order_repository_save_all_to_file(const OrderRegistry* registry, const char* file_path) {
    cJSON* root;
    cJSON* orders;
    size_t i;
    size_t n;
    char* text;
    FILE* fp;
    if (registry == NULL || file_path == NULL) { return 0;
}
    root = cJSON_CreateObject();
    orders = cJSON_CreateArray();
    if (root == NULL || orders == NULL) { cJSON_Delete(root); cJSON_Delete(orders); return 0; }
    cJSON_AddItemToObject(root, "orders", orders);
    n = order_registry_size(registry);
    for (i = 0; i < n; ++i) {
        cJSON* item = order_to_json(order_registry_get_at(registry, i));
        if (item == NULL) { cJSON_Delete(root); return 0; }
        cJSON_AddItemToArray(orders, item);
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

int order_repository_load_all_from_file(OrderRegistry* registry, const char* file_path) {
    FILE* fp;
    long fsize;
    char* text;
    size_t readn;
    cJSON* root;
    cJSON* orders;
    cJSON* item;
    if (registry == NULL || file_path == NULL) { return 0;
}
    fp = fopen(file_path, "rb");
    if (fp == NULL) { return 0;
}
    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); return 0; }
    fsize = ftell(fp); if (fsize < 0) { fclose(fp); return 0; }
    if (fseek(fp, 0, SEEK_SET) != 0) { fclose(fp); return 0; }
    text = (char*)malloc((size_t)fsize + 1); if (text == NULL) { fclose(fp); return 0; }
    readn = fread(text, 1, (size_t)fsize, fp); fclose(fp);
    if (readn != (size_t)fsize) { free(text); return 0; }
    text[fsize] = '\0';
    root = cJSON_Parse(text); free(text); if (root == NULL) { return 0;
}
    orders = cJSON_GetObjectItemCaseSensitive(root, "orders");
    if (!cJSON_IsArray(orders)) { cJSON_Delete(root); return 0; }
    order_registry_clear(registry);
    cJSON_ArrayForEach(item, orders) {
        Order* order = NULL;
        if (!json_to_order(item, &order) || !order_registry_register(registry, order)) {
            order_destroy(order);
            cJSON_Delete(root);
            return 0;
        }
    }
    cJSON_Delete(root);
    return 1;
}
