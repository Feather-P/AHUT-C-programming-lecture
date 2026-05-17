#ifndef PRICE_SERVICE_H
#define PRICE_SERVICE_H

#include "service_common.h"
#include "../models/price/price.h"

typedef struct PriceService {
    PriceHistory* price_history;
} PriceService;

ServiceCode price_service_init(PriceService* svc, PriceHistory* price_history);
ServiceCode price_service_set_market_price(PriceService* svc,
                                           int room_type_id,
                                           double amount,
                                           time_t effective_time);
ServiceCode price_service_get_current_market_price(PriceService* svc,
                                                   int room_type_id,
                                                   double* out_price);

#endif
