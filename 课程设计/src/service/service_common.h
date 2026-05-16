#ifndef SERVICE_COMMON_H
#define SERVICE_COMMON_H

#include <time.h>

typedef enum ServiceCode {
    SERVICE_OK = 0,
    SERVICE_ERR_INVALID_ARG,
    SERVICE_ERR_NOT_FOUND,
    SERVICE_ERR_CONFLICT,
    SERVICE_ERR_INVALID_STATE,
    SERVICE_ERR_PERSISTENCE,
    SERVICE_ERR_INTERNAL
} ServiceCode;

typedef struct DateRange {
    time_t start_inclusive;
    time_t end_exclusive;
} DateRange;

#endif
