#include "insight_service.h"

#include <string.h>

static int in_range(time_t t, DateRange range) {
    return t >= range.start_inclusive && t < range.end_exclusive;
}

static Order* clone_order(const Order* src) {
    if (src == NULL) return NULL;
    Order* o = order_create(order_get_id(src),
                            order_get_guest_prc_citizen_id(src),
                            order_get_room_id(src),
                            order_get_schedule_checkin_date(src),
                            order_get_schedule_checkout_date(src),
                            order_get_cost(src));
    if (o == NULL) return NULL;
    order_set_actual_checkin_time(o, order_get_actual_checkin_time(src));
    order_set_actual_checkout_time(o, order_get_actual_checkout_time(src));
    if (!order_set_status_for_restore(o, order_get_status(src))) {
        order_destroy(o);
        return NULL;
    }
    return o;
}

ServiceCode insight_service_init(InsightService* svc,
                                 const RoomRegistry* room_registry,
                                 const GuestRegistry* guest_registry,
                                 const OrderRegistry* order_registry,
                                 const RoomTypeRegistry* room_type_registry) {
    if (svc == NULL || room_registry == NULL || guest_registry == NULL || order_registry == NULL || room_type_registry == NULL) {
        return SERVICE_ERR_INVALID_ARG;
    }
    svc->room_registry = room_registry;
    svc->guest_registry = guest_registry;
    svc->order_registry = order_registry;
    svc->room_type_registry = room_type_registry;
    return SERVICE_OK;
}

ServiceCode insight_service_query_room_history(InsightService* svc, int room_id, OrderRegistry* out_orders) {
    size_t i, n;
    if (svc == NULL || out_orders == NULL || room_id <= 0) return SERVICE_ERR_INVALID_ARG;
    n = order_registry_size(svc->order_registry);
    for (i = 0; i < n; ++i) {
        Order* o = order_registry_get_at(svc->order_registry, i);
        if (o != NULL && order_get_room_id(o) == room_id) {
            Order* c = clone_order(o);
            if (c == NULL || !order_registry_register(out_orders, c)) {
                if (c != NULL) order_destroy(c);
                return SERVICE_ERR_INTERNAL;
            }
        }
    }
    return SERVICE_OK;
}

ServiceCode insight_service_query_guest_history(InsightService* svc, const char* id_card, OrderRegistry* out_orders) {
    size_t i, n;
    if (svc == NULL || id_card == NULL || out_orders == NULL) return SERVICE_ERR_INVALID_ARG;
    n = order_registry_size(svc->order_registry);
    for (i = 0; i < n; ++i) {
        Order* o = order_registry_get_at(svc->order_registry, i);
        const char* gid;
        if (o == NULL) continue;
        gid = order_get_guest_prc_citizen_id(o);
        if (gid != NULL && strcmp(gid, id_card) == 0) {
            Order* c = clone_order(o);
            if (c == NULL || !order_registry_register(out_orders, c)) {
                if (c != NULL) order_destroy(c);
                return SERVICE_ERR_INTERNAL;
            }
        }
    }
    return SERVICE_OK;
}

ServiceCode insight_service_query_daily_arrivals_departures(InsightService* svc,
                                                             time_t date,
                                                             OrderRegistry* out_arrivals,
                                                             OrderRegistry* out_departures) {
    size_t i, n;
    struct tm date_tm_buf;
    struct tm* date_tm;
    if (svc == NULL || out_arrivals == NULL || out_departures == NULL) return SERVICE_ERR_INVALID_ARG;
    date_tm = localtime(&date);
    if (date_tm == NULL) return SERVICE_ERR_INVALID_ARG;
    date_tm_buf = *date_tm;
    n = order_registry_size(svc->order_registry);
    for (i = 0; i < n; ++i) {
        Order* o = order_registry_get_at(svc->order_registry, i);
        time_t in_t, out_t;
        struct tm* in_tm;
        struct tm* out_tm;
        if (o == NULL) continue;
        in_t = order_get_actual_checkin_time(o);
        out_t = order_get_actual_checkout_time(o);
        if (in_t > 0) {
            in_tm = localtime(&in_t);
            if (in_tm != NULL && in_tm->tm_year == date_tm_buf.tm_year && in_tm->tm_yday == date_tm_buf.tm_yday) {
                Order* c = clone_order(o);
                if (c == NULL || !order_registry_register(out_arrivals, c)) { if (c) order_destroy(c); return SERVICE_ERR_INTERNAL; }
            }
        }
        if (out_t > 0) {
            out_tm = localtime(&out_t);
            if (out_tm != NULL && out_tm->tm_year == date_tm_buf.tm_year && out_tm->tm_yday == date_tm_buf.tm_yday) {
                Order* c = clone_order(o);
                if (c == NULL || !order_registry_register(out_departures, c)) { if (c) order_destroy(c); return SERVICE_ERR_INTERNAL; }
            }
        }
    }
    return SERVICE_OK;
}

ServiceCode insight_service_occupancy_snapshot(InsightService* svc, time_t as_of, OccupancySnapshot* out_snapshot) {
    size_t i, n, rn;
    int by_count;
    int occupied = 0;
    int total = 0;
    int occ_by[16] = {0};
    int total_by[16] = {0};
    time_t ref_time;
    if (svc == NULL) return SERVICE_ERR_INVALID_ARG;
    if (out_snapshot == NULL) return SERVICE_ERR_INVALID_ARG;
    ref_time = (as_of > 0) ? as_of : time(NULL);
    rn = room_type_registry_size(svc->room_type_registry);
    by_count = (rn > 16) ? 16 : (int)rn;
    n = room_registry_size(svc->room_registry);
    for (i = 0; i < n; ++i) {
        Room* r = room_registry_get_at(svc->room_registry, i);
        if (r == NULL) continue;
        total++;
        if (room_get_status(r) == ROOM_STATUS_OCCUPIED ||
            (room_get_status(r) == ROOM_STATUS_IDLE && room_get_last_checkout_time(r) > ref_time)) {
            occupied++;
        }
        if (room_get_type_id(r) >= 1 && room_get_type_id(r) <= by_count) {
            int idx = room_get_type_id(r) - 1;
            total_by[idx]++;
            if (room_get_status(r) == ROOM_STATUS_OCCUPIED ||
                (room_get_status(r) == ROOM_STATUS_IDLE && room_get_last_checkout_time(r) > ref_time)) {
                occ_by[idx]++;
            }
        }
    }
    out_snapshot->overall_occupancy = (total == 0) ? 0.0 : ((double)occupied / (double)total);
    out_snapshot->by_room_type_count = by_count;
    for (i = 0; i < (size_t)by_count; ++i) {
        out_snapshot->by_room_type[i] = (total_by[i] == 0) ? 0.0 : ((double)occ_by[i] / (double)total_by[i]);
    }
    return SERVICE_OK;
}

ServiceCode insight_service_top10_vip_by_month(InsightService* svc, int year, int month, GuestRegistry* out_top10) {
    size_t i, gn;
    int counts[512] = {0};
    int idxs[512];
    int valid = 0;
    if (svc == NULL || out_top10 == NULL || month < 1 || month > 12) return SERVICE_ERR_INVALID_ARG;
    gn = guest_registry_size(svc->guest_registry);
    if (gn > 512) gn = 512;
    for (i = 0; i < gn; ++i) idxs[i] = (int)i;
    for (i = 0; i < order_registry_size(svc->order_registry); ++i) {
        Order* o = order_registry_get_at(svc->order_registry, i);
        time_t t;
        struct tm* lt;
        size_t j;
        if (o == NULL) continue;
        t = order_get_actual_checkin_time(o);
        if (t <= 0) continue;
        lt = localtime(&t);
        if (lt == NULL || lt->tm_year + 1900 != year || lt->tm_mon + 1 != month) continue;
        for (j = 0; j < gn; ++j) {
            Guest* g = guest_registry_get_at(svc->guest_registry, j);
            if (g != NULL && strcmp(guest_get_prc_citizen_id(g), order_get_guest_prc_citizen_id(o)) == 0) {
                counts[j]++;
                break;
            }
        }
    }
    for (i = 0; i < gn; ++i) {
        size_t j;
        for (j = i + 1; j < gn; ++j) {
            if (counts[idxs[j]] > counts[idxs[i]]) { int t = idxs[i]; idxs[i] = idxs[j]; idxs[j] = t; }
        }
    }
    for (i = 0; i < gn && valid < 10; ++i) {
        Guest* src;
        Guest* c;
        if (counts[idxs[i]] <= 0) continue;
        src = guest_registry_get_at(svc->guest_registry, (size_t)idxs[i]);
        if (src == NULL) continue;
        c = guest_create(guest_get_id(src), guest_get_prc_citizen_id(src), guest_get_name(src), guest_get_tel(src), guest_get_sex(src), guest_get_checkin_count(src));
        if (c == NULL || !guest_registry_register(out_top10, c)) { if (c) guest_destroy(c); return SERVICE_ERR_INTERNAL; }
        valid++;
    }
    return SERVICE_OK;
}

ServiceCode insight_service_revenue_report(InsightService* svc,
                                           DateRange range,
                                           double* out_total,
                                           double by_room_type[16],
                                           int* out_type_count) {
    size_t i;
    if (svc == NULL || by_room_type == NULL) return SERVICE_ERR_INVALID_ARG;
    if (out_total == NULL || out_type_count == NULL) return SERVICE_ERR_INVALID_ARG;
    if (range.end_exclusive <= range.start_inclusive) return SERVICE_ERR_INVALID_ARG;
    *out_total = 0.0;
    memset(by_room_type, 0, sizeof(double) * 16);
    *out_type_count = (int)room_type_registry_size(svc->room_type_registry);
    if (*out_type_count > 16) *out_type_count = 16;
    for (i = 0; i < order_registry_size(svc->order_registry); ++i) {
        Order* o = order_registry_get_at(svc->order_registry, i);
        int rid;
        Room* r;
        int tid;
        if (o == NULL || order_get_status(o) != ORDER_STATUS_CHECKED_OUT) continue;
        if (!in_range(order_get_actual_checkout_time(o), range)) continue;
        *out_total += order_get_cost(o);
        rid = order_get_room_id(o);
        r = room_registry_find_by_id(svc->room_registry, rid);
        if (r == NULL) continue;
        tid = room_get_type_id(r);
        if (tid >= 1 && tid <= 16) by_room_type[tid - 1] += order_get_cost(o);
    }
    return SERVICE_OK;
}

ServiceCode insight_service_longest_vacant_rooms(InsightService* svc, time_t as_of, RoomRegistry* out_rooms) {
    size_t i, n;
    Room* best[10] = {0};
    time_t best_gap[10] = {0};
    if (svc == NULL || out_rooms == NULL) return SERVICE_ERR_INVALID_ARG;
    n = room_registry_size(svc->room_registry);
    for (i = 0; i < n; ++i) {
        Room* r = room_registry_get_at(svc->room_registry, i);
        time_t gap;
        int k;
        if (r == NULL || room_get_status(r) != ROOM_STATUS_IDLE) continue;
        gap = as_of - room_get_last_checkout_time(r);
        if (gap < 0) gap = 0;
        for (k = 0; k < 10; ++k) {
            if (best[k] == NULL || gap > best_gap[k]) {
                int m;
                for (m = 9; m > k; --m) { best[m] = best[m - 1]; best_gap[m] = best_gap[m - 1]; }
                best[k] = r;
                best_gap[k] = gap;
                break;
            }
        }
    }
    for (i = 0; i < 10; ++i) {
        Room* src;
        Room* c;
        if (best[i] == NULL) continue;
        src = best[i];
        c = room_create(room_get_id(src), room_get_no(src), room_get_floor(src), room_get_bed_count(src), room_get_area(src), room_get_description(src), room_get_type_id(src), room_get_status(src), room_get_created_at(src), room_get_updated_at(src), room_get_last_checkout_time(src));
        if (c == NULL || !room_registry_register(out_rooms, c)) { if (c) room_destroy(c); return SERVICE_ERR_INTERNAL; }
    }
    return SERVICE_OK;
}

ServiceCode insight_service_guest_source_ratio(InsightService* svc,
                                               DateRange range,
                                               double* out_returning_ratio,
                                               double* out_new_ratio) {
    size_t i;
    int returning = 0, newer = 0, total = 0;
    if (svc == NULL) return SERVICE_ERR_INVALID_ARG;
    if (out_returning_ratio == NULL || out_new_ratio == NULL) return SERVICE_ERR_INVALID_ARG;
    if (range.end_exclusive <= range.start_inclusive) return SERVICE_ERR_INVALID_ARG;
    for (i = 0; i < order_registry_size(svc->order_registry); ++i) {
        Order* o = order_registry_get_at(svc->order_registry, i);
        Guest* g;
        if (o == NULL) continue;
        if (!in_range(order_get_actual_checkin_time(o), range)) continue;
        g = guest_registry_find_by_prc_citizen_id(svc->guest_registry, order_get_guest_prc_citizen_id(o));
        if (g == NULL) continue;
        total++;
        if (guest_get_checkin_count(g) > 1) returning++;
        else newer++;
    }
    *out_returning_ratio = (total == 0) ? 0.0 : ((double)returning / (double)total);
    *out_new_ratio = (total == 0) ? 0.0 : ((double)newer / (double)total);
    return SERVICE_OK;
}
