#include "tui_app.h"

#include "../models/guest/guest.h"
#include "../models/order/order.h"
#include "../models/price/price.h"
#include "../models/room/room.h"
#include "../models/room/room_type.h"
#include "../config/config.h"
#include "../repository/repository.h"
#include "../service/guest_profile_service.h"
#include "../service/insight_service.h"
#include "../service/inventory_service.h"
#include "../service/stay_lifecycle_service.h"

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct AppCtx {
    RoomRegistry* rr;
    RoomTypeRegistry* tr;
    GuestRegistry* gr;
    OrderRegistry* orr;
    PriceHistory* ph;
    InventoryService inv;
    GuestProfileService gps;
    StayLifecycleService stay;
    InsightService ins;
} AppCtx;

static void msg(const char* s) { printf("%s\n", s); }

static int read_line(char* out, size_t n) {
    if (fgets(out, (int)n, stdin) == NULL) return 0;
    size_t len = strlen(out);
    if (len > 0 && out[len - 1] == '\n') out[len - 1] = '\0';
    return 1;
}

static int in_i(const char* p) {
    char b[64];
    printf("%s ", p);
    if (!read_line(b, sizeof(b))) return 0;
    return atoi(b);
}

static double in_d(const char* p) {
    char b[64];
    printf("%s ", p);
    if (!read_line(b, sizeof(b))) return 0.0;
    return atof(b);
}

static void in_s(const char* p, char* out, int n) {
    printf("%s ", p);
    if (!read_line(out, (size_t)n)) {
        if (n > 0) out[0] = '\0';
    }
}

static time_t now_day(void) { time_t t = time(NULL); struct tm* m = localtime(&t); m->tm_hour=0; m->tm_min=0; m->tm_sec=0; return mktime(m); }

static void seed(AppCtx* c) {
    room_type_registry_register(c->tr, room_type_create(1, "Single"));
    room_type_registry_register(c->tr, room_type_create(2, "Double"));
    room_registry_register(c->rr, room_create(101, "101", 1, 1, 20, "single", 1, ROOM_STATUS_IDLE, time(NULL), time(NULL), 0));
    room_registry_register(c->rr, room_create(102, "102", 1, 2, 30, "double", 2, ROOM_STATUS_IDLE, time(NULL), time(NULL), 0));
    guest_registry_register(c->gr, guest_create(1, "340100199001010011", "Alice", "13800000001", SEX_FEMALE, 0));
    price_history_append(c->ph, price_create(1, 199, time(NULL)));
    price_history_append(c->ph, price_create(2, 299, time(NULL)));
}

static int load_from_repository(AppCtx* c) {
    int ok = 1;
    ok = ok && room_type_repository_load_all_from_file(c->tr, config_get_room_type_file_path());
    ok = ok && room_repository_load_all_from_file(c->rr, config_get_room_file_path());
    ok = ok && guest_repository_load_all_from_file(c->gr, config_get_guest_file_path());
    ok = ok && order_repository_load_all_from_file(c->orr, config_get_order_file_path());
    ok = ok && price_repository_load_all_from_file(c->ph, config_get_price_file_path());
    return ok;
}

static void do_inventory(AppCtx* c) {
    int op = in_i("Inventory:1 add 2 upd 3 rm 4 reserved 5 occupied 6 vacant 7 maint 8 list");
    if (op == 1) {
        int id = in_i("room_id="); int type = in_i("type_id=");
        Room* r = room_create(id, "X", 1, 1, 20, "", type, ROOM_STATUS_IDLE, time(NULL), time(NULL), 0);
        msg(inventory_service_add_room(&c->inv, r) == SERVICE_OK ? "OK" : "FAIL");
    } else if (op == 2) {
        int id = in_i("room_id="); Room* r = room_registry_find_by_id(c->rr, id);
        msg((r && inventory_service_update_room(&c->inv, r) == SERVICE_OK) ? "OK" : "FAIL");
    } else if (op == 3) msg(inventory_service_remove_room(&c->inv, in_i("room_id=")) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 4) msg(inventory_service_mark_reserved(&c->inv, in_i("room_id=")) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 5) msg(inventory_service_mark_occupied(&c->inv, in_i("room_id=")) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 6) msg(inventory_service_mark_vacant(&c->inv, in_i("room_id=")) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 7) msg(inventory_service_mark_maintenance(&c->inv, in_i("room_id=")) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 8) {
        RoomRegistry* out = room_registry_create();
        int type = in_i("type_id=");
        inventory_service_list_available_by_type(&c->inv, type, out);
        char t[64]; snprintf(t, sizeof(t), "available=%zu", room_registry_size(out)); msg(t);
    }
}

static void do_guest(AppCtx* c) {
    int op = in_i("Guest:1 add 2 upd 3 rm 4 find 5 inc");
    if (op == 1) {
        int id = in_i("id="); char cid[24], name[32], tel[24]; in_s("id_card=", cid, 24); in_s("name=", name, 32); in_s("tel=", tel, 24);
        Guest* g = guest_create(id, cid, name, tel, SEX_UNKNOWN, 0);
        msg(guest_profile_service_add_guest(&c->gps, g) == SERVICE_OK ? "OK" : "FAIL");
    } else if (op == 2) {
        int id = in_i("id="); Guest* g = guest_registry_find_by_id(c->gr, id);
        msg((g && guest_profile_service_update_guest(&c->gps, g) == SERVICE_OK) ? "OK" : "FAIL");
    } else if (op == 3) msg(guest_profile_service_remove_guest(&c->gps, in_i("id=")) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 4) {
        char cid[24]; Guest* g = NULL; in_s("id_card=", cid, 24);
        msg(guest_profile_service_find_by_id_card(&c->gps, cid, &g) == SERVICE_OK ? "FOUND" : "NOT FOUND");
    } else if (op == 5) {
        char cid[24]; in_s("id_card=", cid, 24);
        msg(guest_profile_service_increment_stay_count(&c->gps, cid) == SERVICE_OK ? "OK" : "FAIL");
    }
}

static void do_stay(AppCtx* c) {
    int op = in_i("Stay:1 reserve 2 cancel 3 in 4 extend 5 change 6 out 7 nights 8 amount");
    if (op == 1) {
        CreateReservationCmd cmd; memset(&cmd, 0, sizeof(cmd));
        cmd.order_id = in_i("order_id="); in_s("guest_id_card=", cmd.guest_id_card, 24); cmd.room_id = in_i("room_id=");
        cmd.reserved_at = time(NULL); cmd.checkin_date = now_day(); cmd.checkout_date = now_day() + 86400;
        msg(stay_service_create_reservation(&c->stay, &cmd) == SERVICE_OK ? "OK" : "FAIL");
    } else if (op == 2) msg(stay_service_cancel_reservation(&c->stay, in_i("order_id=")) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 3) msg(stay_service_check_in(&c->stay, in_i("order_id="), time(NULL)) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 4) msg(stay_service_extend_checkout_date(&c->stay, in_i("order_id="), now_day() + 2 * 86400) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 5) msg(stay_service_change_room(&c->stay, in_i("order_id="), in_i("new_room_id=")) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 6) { SettlementResult s; msg(stay_service_check_out_and_settle(&c->stay, in_i("order_id="), time(NULL)+3600, &s) == SERVICE_OK ? "OK" : "FAIL"); }
    else if (op == 7) { int n=0; msg(stay_service_calculate_nights(now_day(), now_day()+86400, &n) == SERVICE_OK ? "OK" : "FAIL"); }
    else if (op == 8) { double a,u; msg(stay_service_calculate_amount(&c->stay, in_i("room_type_id="), in_i("nights="), &a, &u) == SERVICE_OK ? "OK" : "FAIL"); }
}

static void do_insight(AppCtx* c) {
    int op = in_i("Insight:1 room_history 2 guest_history 3 arrivals_departures 4 occupancy 5 vip10 6 revenue 7 vacant 8 source_ratio");
    OrderRegistry* o1 = order_registry_create(); OrderRegistry* o2 = order_registry_create(); GuestRegistry* g = guest_registry_create(); RoomRegistry* r = room_registry_create();
    OccupancySnapshot s; double total=0, by[16], rr=0,nr=0; int cnt=0;
    if (op == 1) msg(insight_service_query_room_history(&c->ins, in_i("room_id="), o1) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 2) { char cid[24]; in_s("id_card=", cid, 24); msg(insight_service_query_guest_history(&c->ins, cid, o1) == SERVICE_OK ? "OK" : "FAIL"); }
    else if (op == 3) msg(insight_service_query_daily_arrivals_departures(&c->ins, now_day(), o1, o2) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 4) msg(insight_service_occupancy_snapshot(&c->ins, time(NULL), &s) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 5) msg(insight_service_top10_vip_by_month(&c->ins, 2026, 5, g) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 6) msg(insight_service_revenue_report(&c->ins, (DateRange){now_day()-86400, now_day()+86400}, &total, by, &cnt) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 7) msg(insight_service_longest_vacant_rooms(&c->ins, time(NULL), r) == SERVICE_OK ? "OK" : "FAIL");
    else if (op == 8) msg(insight_service_guest_source_ratio(&c->ins, (DateRange){now_day()-86400, now_day()+86400}, &rr, &nr) == SERVICE_OK ? "OK" : "FAIL");
}

void tui_app_run(void) {
    AppCtx c;
    int run = 1;
    setlocale(LC_ALL, "");
    memset(&c, 0, sizeof(c));
    config_load();
    price_repository_set_file_path(config_get_price_file_path());
    c.rr = room_registry_create(); c.tr = room_type_registry_create(); c.gr = guest_registry_create(); c.orr = order_registry_create(); c.ph = price_history_create();
    inventory_service_init(&c.inv, c.rr, c.tr);
    guest_profile_service_init(&c.gps, c.gr);
    stay_service_init(&c.stay, c.orr, c.ph, &c.inv, &c.gps);
    insight_service_init(&c.ins, c.rr, c.gr, c.orr, c.tr);
    if (!load_from_repository(&c)) {
        seed(&c);
    }

    while (run) {
        char ch[8] = {0};
        printf("\n==== Service Menu ====\n");
        printf("1. InventoryService\n");
        printf("2. GuestProfileService\n");
        printf("3. StayLifecycleService\n");
        printf("4. InsightService\n");
        printf("q. Quit\n");
        printf("Choose: ");
        if (!read_line(ch, sizeof(ch))) break;

        if (ch[0] == '1') { do_inventory(&c); }
        else if (ch[0] == '2') { do_guest(&c); }
        else if (ch[0] == '3') { do_stay(&c); }
        else if (ch[0] == '4') { do_insight(&c); }
        else if (ch[0] == 'q' || ch[0] == 'Q') run = 0;
        else msg("Invalid choice");
    }
}
