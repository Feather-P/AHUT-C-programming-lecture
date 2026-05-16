#ifndef PRICE_H
#define PRICE_H

#include <stddef.h>
#include <time.h>

typedef struct Price Price;
typedef struct PriceHistory PriceHistory;

Price* price_create(int room_type_id, double amount, time_t effective_time);
void price_destroy(Price* price);

int price_get_room_type_id(const Price* price);
double price_get_amount(const Price* price);
time_t price_get_effective_time(const Price* price);

PriceHistory* price_history_create(void);
void price_history_destroy(PriceHistory* history);
void price_history_clear(PriceHistory* history);

int price_history_append(PriceHistory* history, Price* price);
size_t price_history_size(const PriceHistory* history);
Price* price_history_get_at(const PriceHistory* history, size_t index);
Price* price_history_get_latest_by_room_type_id(const PriceHistory* history, int room_type_id);

#endif
