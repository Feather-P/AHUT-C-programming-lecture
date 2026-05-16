#include "price.h"

#include "../../utils/linked_list.h"

#include <stdlib.h>

struct Price {
    int room_type_id;
    double amount;
    time_t effective_time;
};

struct PriceHistory {
    LinkedList* records;
};

Price* price_create(int room_type_id, double amount, time_t effective_time) {
    Price* price;
    if (amount < 0.0) {
        return NULL;
    }

    price = (Price*)malloc(sizeof(Price));
    if (price == NULL) {
        return NULL;
    }

    price->room_type_id = room_type_id;
    price->amount = amount;
    price->effective_time = effective_time;
    return price;
}

void price_destroy(Price* price) {
    if (price == NULL) {
        return;
    }
    free(price);
}

int price_get_room_type_id(const Price* price) { return price ? price->room_type_id : -1; }
double price_get_amount(const Price* price) { return price ? price->amount : 0.0; }
time_t price_get_effective_time(const Price* price) { return price ? price->effective_time : (time_t)0; }

PriceHistory* price_history_create(void) {
    PriceHistory* history = (PriceHistory*)malloc(sizeof(PriceHistory));
    if (history == NULL) {
        return NULL;
    }

    history->records = linked_list_create((linked_list_free_fn)price_destroy);
    if (history->records == NULL) {
        free(history);
        return NULL;
    }
    return history;
}

void price_history_destroy(PriceHistory* history) {
    if (history == NULL) {
        return;
    }
    linked_list_destroy(history->records);
    free(history);
}

void price_history_clear(PriceHistory* history) {
    if (history == NULL || history->records == NULL) {
        return;
    }
    linked_list_clear(history->records);
}

int price_history_append(PriceHistory* history, Price* price) {
    if (history == NULL || history->records == NULL || price == NULL) {
        return 0;
    }
    return linked_list_push_back(history->records, price);
}

size_t price_history_size(const PriceHistory* history) {
    if (history == NULL || history->records == NULL) {
        return 0;
    }
    return linked_list_size(history->records);
}

Price* price_history_get_at(const PriceHistory* history, size_t index) {
    if (history == NULL || history->records == NULL) {
        return NULL;
    }
    return (Price*)linked_list_get(history->records, index);
}

Price* price_history_get_latest_by_room_type_id(const PriceHistory* history, int room_type_id) {
    size_t i;
    size_t n;
    Price* latest = NULL;

    if (history == NULL || history->records == NULL) {
        return NULL;
    }

    n = linked_list_size(history->records);
    for (i = 0; i < n; ++i) {
        Price* item = (Price*)linked_list_get(history->records, i);
        if (item != NULL && price_get_room_type_id(item) == room_type_id) {
            latest = item;
        }
    }

    return latest;
}
