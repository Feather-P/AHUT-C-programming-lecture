#include "price_service.h"

#include "../repository/repository.h"

ServiceCode price_service_init(PriceService* svc, PriceHistory* price_history) {
    if (svc == NULL || price_history == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    svc->price_history = price_history;
    return SERVICE_OK;
}

ServiceCode price_service_set_market_price(PriceService* svc,
                                           int room_type_id,
                                           double amount,
                                           time_t effective_time) {
    if (svc == NULL || svc->price_history == NULL || room_type_id <= 0 || amount < 0.0) {
        return SERVICE_ERR_INVALID_ARG;
    }
    if (!price_repository_append_and_persist(svc->price_history, room_type_id, amount, effective_time, NULL)) {
        return SERVICE_ERR_PERSISTENCE;
    }
    return SERVICE_OK;
}

ServiceCode price_service_get_current_market_price(PriceService* svc,
                                                   int room_type_id,
                                                   double* out_price) {
    if (svc == NULL || svc->price_history == NULL || room_type_id <= 0 || out_price == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    if (!price_repository_get_current_market_price(svc->price_history, room_type_id, out_price)) {
        return SERVICE_ERR_NOT_FOUND;
    }
    return SERVICE_OK;
}
