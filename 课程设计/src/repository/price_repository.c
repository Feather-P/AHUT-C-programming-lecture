#include "repository.h"

#include "../models/price/price.h"

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* g_price_repository_file_path = "data/prices.json";

static cJSON* price_to_json(const Price* price) {
    cJSON* obj;
    if (price == NULL) {
        return NULL;
    }

    obj = cJSON_CreateObject();
    if (obj == NULL) {
        return NULL;
    }

    if (!cJSON_AddNumberToObject(obj, "room_type_id", price_get_room_type_id(price)) ||
        !cJSON_AddNumberToObject(obj, "amount", price_get_amount(price)) ||
        !cJSON_AddNumberToObject(obj, "effective_time", (double)price_get_effective_time(price))) {
        cJSON_Delete(obj);
        return NULL;
    }

    return obj;
}

static int json_to_price(const cJSON* obj, Price** out_price) {
    const cJSON* room_type_id;
    const cJSON* amount;
    const cJSON* effective_time;
    Price* price;

    if (obj == NULL || out_price == NULL) {
        return 0;
    }

    room_type_id = cJSON_GetObjectItemCaseSensitive(obj, "room_type_id");
    amount = cJSON_GetObjectItemCaseSensitive(obj, "amount");
    effective_time = cJSON_GetObjectItemCaseSensitive(obj, "effective_time");
    if (!cJSON_IsNumber(room_type_id) || !cJSON_IsNumber(amount) || !cJSON_IsNumber(effective_time)) {
        return 0;
    }

    price = price_create(room_type_id->valueint, amount->valuedouble, (time_t)effective_time->valuedouble);
    if (price == NULL) {
        return 0;
    }

    *out_price = price;
    return 1;
}

int price_repository_set_file_path(const char* file_path) {
    if (file_path == NULL || file_path[0] == '\0') {
        return 0;
    }
    g_price_repository_file_path = file_path;
    return 1;
}

const char* price_repository_get_file_path(void) { return g_price_repository_file_path; }

int price_repository_save_all_to_file(const PriceHistory* history, const char* file_path) {
    size_t i;
    size_t n;
    cJSON* root;
    cJSON* prices;
    char* text;
    FILE* fp;

    if (history == NULL || file_path == NULL) {
        return 0;
    }

    root = cJSON_CreateObject();
    prices = cJSON_CreateArray();
    if (root == NULL || prices == NULL) {
        cJSON_Delete(root);
        cJSON_Delete(prices);
        return 0;
    }
    cJSON_AddItemToObject(root, "prices", prices);

    n = price_history_size(history);
    for (i = 0; i < n; ++i) {
        Price* price = price_history_get_at(history, i);
        cJSON* item = price_to_json(price);
        if (item == NULL) {
            cJSON_Delete(root);
            return 0;
        }
        cJSON_AddItemToArray(prices, item);
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

int price_repository_load_all_from_file(PriceHistory* history, const char* file_path) {
    FILE* fp;
    long fsize;
    char* text;
    size_t readn;
    cJSON* root;
    cJSON* prices;
    cJSON* item;

    if (history == NULL || file_path == NULL) {
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

    prices = cJSON_GetObjectItemCaseSensitive(root, "prices");
    if (!cJSON_IsArray(prices)) {
        cJSON_Delete(root);
        return 0;
    }

    price_history_clear(history);
    cJSON_ArrayForEach(item, prices) {
        Price* price = NULL;
        if (!json_to_price(item, &price) || !price_history_append(history, price)) {
            price_destroy(price);
            cJSON_Delete(root);
            return 0;
        }
    }

    cJSON_Delete(root);
    return 1;
}

int price_repository_append_and_persist(PriceHistory* history,
                                        int room_type_id,
                                        double amount,
                                        time_t effective_time,
                                        const char* file_path) {
    Price* price;
    const char* target_path;

    if (history == NULL || amount < 0.0) {
        return 0;
    }

    target_path = (file_path == NULL) ? g_price_repository_file_path : file_path;
    price = price_create(room_type_id, amount, effective_time);
    if (price == NULL) {
        return 0;
    }

    if (!price_history_append(history, price)) {
        price_destroy(price);
        return 0;
    }

    if (!price_repository_save_all_to_file(history, target_path)) {
        return 0;
    }

    return 1;
}

int price_repository_get_current_market_price(const PriceHistory* history, int room_type_id, double* out_price) {
    Price* latest;
    if (history == NULL || out_price == NULL) {
        return 0;
    }

    latest = price_history_get_latest_by_room_type_id(history, room_type_id);
    if (latest == NULL) {
        return 0;
    }

    *out_price = price_get_amount(latest);
    return 1;
}
