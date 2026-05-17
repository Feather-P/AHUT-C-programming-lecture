#include "tui_app.h"

#include "../config/config.h"
#include "../repository/repository.h"
#include "../service/guest_profile_service.h"
#include "../service/insight_service.h"
#include "../service/inventory_service.h"
#include "../service/price_service.h"
#include "../service/stay_lifecycle_service.h"
#include "../utils/logger.h"

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
    PriceService ps;
    StayLifecycleService stay;
    InsightService ins;
} AppCtx;

typedef struct UiState {
    char current_module[32];
    char current_object[96];
    char last_result[160];
} UiState;

static void msg(const char* s) { printf("%s\n", s); }

static const char* room_status_to_string(RoomStatus status) {
    switch (status) {
        case ROOM_STATUS_IDLE: return "空闲";
        case ROOM_STATUS_RESERVED: return "已预订";
        case ROOM_STATUS_OCCUPIED: return "已入住";
        case ROOM_STATUS_MAINTENANCE: return "维护中";
        default: return "未知房态";
    }
}

static const char* order_status_to_string(OrderStatus status) {
    switch (status) {
        case ORDER_STATUS_RESERVED: return "已预订";
        case ORDER_STATUS_CANCELLED: return "已取消";
        case ORDER_STATUS_CHECKED_IN: return "已入住";
        case ORDER_STATUS_CHECKED_OUT: return "已退房";
        default: return "未知订单状态";
    }
}

static const char* service_code_to_string(ServiceCode code) {
    switch (code) {
        case SERVICE_OK: return "SERVICE_OK";
        case SERVICE_ERR_INVALID_ARG: return "SERVICE_ERR_INVALID_ARG";
        case SERVICE_ERR_NOT_FOUND: return "SERVICE_ERR_NOT_FOUND";
        case SERVICE_ERR_CONFLICT: return "SERVICE_ERR_CONFLICT";
        case SERVICE_ERR_INVALID_STATE: return "SERVICE_ERR_INVALID_STATE";
        case SERVICE_ERR_PERSISTENCE: return "SERVICE_ERR_PERSISTENCE";
        case SERVICE_ERR_INTERNAL: return "SERVICE_ERR_INTERNAL";
        default: return "SERVICE_ERR_UNKNOWN";
    }
}

static void msg_service_result(const char* op_name, ServiceCode code) {
    char buf[96];
    snprintf(buf, sizeof(buf), "%s (%d)", service_code_to_string(code), (int)code);
    msg(buf);
    if (code != SERVICE_OK) {
        LOG_ERROR("UI op failed: %s => %s(%d)", op_name, service_code_to_string(code), (int)code);
    }
}

static void ui_set_last_result(UiState* ui, const char* op_name, ServiceCode code) {
    if (ui == NULL) return;
    snprintf(ui->last_result, sizeof(ui->last_result), "%s => %s(%d)",
             op_name, service_code_to_string(code), (int)code);
}

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

static int in_menu(const char* p, int* out_v) {
    char b[64];
    char* end = NULL;
    long v;
    if (out_v == NULL) return 0;
    printf("%s ", p);
    if (!read_line(b, sizeof(b))) return 0;
    v = strtol(b, &end, 10);
    if (end == b || *end != '\0') return 0;
    *out_v = (int)v;
    return 1;
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

static void ui_render_main(const UiState* ui) {
    printf("\n==============================================\n");
    printf("                 酒店管理控制台               \n");
    printf("==============================================\n");
    printf("最近结果：%s\n", (ui != NULL && ui->last_result[0] != '\0') ? ui->last_result : "-" );
    printf("----------------------------------------------\n");
    printf("[主菜单]\n");
    printf("  1. 房间管理\n");
    printf("  2. 住客档案\n");
    printf("  3. 入住全流程\n");
    printf("  4. 价格管理\n");
    printf("  5. 报表洞察\n");
    printf("  q. 退出\n");
    printf("==============================================\n");
}

static int choose_room_type(AppCtx* c, int* out_type_id) {
    size_t i, n = 0;
    int idx = 0;
    RoomTypeView views[128];
    ServiceCode rc = inventory_service_list_all_room_type_views(&c->inv, views, 128, &n);
    if (rc != SERVICE_OK) return 0;
    if (n == 0) return 0;
    printf("候选房型 (0取消):\n");
    for (i = 0; i < n; ++i) printf("  %zu) %s (type_id=%d)\n", i + 1, views[i].type_name, views[i].type_id);
    if (!in_menu("选择序号:", &idx)) return 0;
    if (idx == 0) return 0;
    if (idx < 1 || (size_t)idx > n) return 0;
    *out_type_id = views[idx - 1].type_id;
    return 1;
}

static int choose_guest_id_card(AppCtx* c, char* out_id_card, size_t out_n) {
    size_t i, n = 0;
    int idx = 0;
    GuestView views[256];
    ServiceCode rc = guest_profile_service_list_all_guest_views(&c->gps, views, 256, &n);
    if (rc != SERVICE_OK || n == 0) return 0;
    printf("候选住客 (0取消):\n");
    for (i = 0; i < n; ++i) printf("  %zu) %s | %s | %s\n", i + 1, views[i].name, views[i].id_card, views[i].tel);
    if (!in_menu("选择序号:", &idx)) return 0;
    if (idx == 0) return 0;
    if (idx < 1 || (size_t)idx > n) return 0;
    snprintf(out_id_card, out_n, "%s", views[idx - 1].id_card);
    return 1;
}

static int choose_room_id(AppCtx* c, int require_idle, int* out_room_id) {
    size_t i, n = 0, show = 0;
    size_t j, rn = 0;
    int idx = 0;
    int map[256];
    RoomView views[256];
    ServiceCode rc = inventory_service_list_all_room_views(&c->inv, views, 256, &n);
    if (rc != SERVICE_OK) return 0;
    printf("候选房间 (0取消):\n");
    for (i = 0; i < n; ++i) {
        if (require_idle && views[i].status != ROOM_STATUS_IDLE) continue;
        Room* room = NULL;
        rn = room_registry_size(c->rr);
        for (j = 0; j < rn; ++j) {
            Room* cur = room_registry_get_at(c->rr, j);
            if (cur != NULL && strcmp(room_get_no(cur), views[i].room_no) == 0) {
                room = cur;
                break;
            }
        }
        if (room == NULL) continue;
        printf("  %zu) 房号=%s type=%s status=%s(%d)\n", show + 1, views[i].room_no, views[i].type_name,
               room_status_to_string(views[i].status), (int)views[i].status);
        map[show++] = room_get_id(room);
    }
    if (show == 0) return 0;
    if (!in_menu("选择序号:", &idx)) return 0;
    if (idx == 0) return 0;
    if (idx < 1 || (size_t)idx > show) return 0;
    *out_room_id = map[idx - 1];
    return 1;
}

static int choose_order_id(AppCtx* c, int status_filter, int* out_order_id) {
    size_t i, n = 0;
    int idx = 0;
    StayOrderView views[256];
    ServiceCode rc;
    if (status_filter < 0) {
        rc = stay_service_list_order_views(&c->stay, views, 256, &n);
    } else {
        rc = stay_service_list_order_views_by_status(&c->stay, (OrderStatus)status_filter, views, 256, &n);
    }
    if (rc != SERVICE_OK || n == 0) return 0;
    printf("候选订单 (0取消):\n");
    for (i = 0; i < n; ++i) printf("  %zu) order_id=%d | guest=%s | room_id=%d | status=%s(%d)\n", i + 1, views[i].order_id, views[i].guest_id_card, views[i].room_id, order_status_to_string(views[i].status), (int)views[i].status);
    if (!in_menu("选择序号:", &idx)) return 0;
    if (idx == 0) return 0;
    if (idx < 1 || (size_t)idx > n) return 0;
    *out_order_id = views[idx - 1].order_id;
    return 1;
}

static void print_room_types(AppCtx* c) {
    size_t i, n = 0;
    RoomTypeView views[128];
    ServiceCode rc = inventory_service_list_all_room_type_views(&c->inv, views, 128, &n);
    msg_service_result("inventory.list_all_room_type_views", rc);
    if (rc != SERVICE_OK) return;
    printf("-- 房型列表 --\n");
    for (i = 0; i < n; ++i) printf("房型ID=%d 房型名称=%s\n", views[i].type_id, views[i].type_name);
}

static void print_guests(AppCtx* c) {
    size_t i, n = 0;
    GuestView views[256];
    ServiceCode rc = guest_profile_service_list_all_guest_views(&c->gps, views, 256, &n);
    msg_service_result("guest.list_all_guest_views", rc);
    if (rc != SERVICE_OK) return;
    printf("-- 住客列表 --\n");
    for (i = 0; i < n; ++i) {
        printf("身份证=%s 姓名=%s 电话=%s 性别=%d 入住次数=%d\n",
               views[i].id_card, views[i].name, views[i].tel,
               (int)views[i].sex, views[i].stay_count);
    }
}

static void print_orders(AppCtx* c) {
    size_t i, n = 0;
    StayOrderView views[256];
    ServiceCode rc = stay_service_list_order_views(&c->stay, views, 256, &n);
    msg_service_result("stay.list_order_views", rc);
    if (rc != SERVICE_OK) return;
    printf("-- 订单列表 --\n");
    for (i = 0; i < n; ++i) {
        printf("订单ID=%d 住客证件=%s 房间ID=%d 状态=%s(%d) 入住时间=%lld 退房时间=%lld\n",
               views[i].order_id, views[i].guest_id_card, views[i].room_id,
               order_status_to_string(views[i].status), (int)views[i].status,
               (long long)views[i].checkin_time, (long long)views[i].checkout_time);
    }
}

static void print_order_registry(const OrderRegistry* registry, const char* title) {
    size_t i, n = 0;
    if (registry == NULL) return;
    n = order_registry_size(registry);
    printf("-- %s（数量=%zu） --\n", title, n);
    for (i = 0; i < n; ++i) {
        Order* o = order_registry_get_at(registry, i);
        if (o == NULL) continue;
        printf("订单ID=%d 住客证件=%s 房间ID=%d 状态=%s(%d) 入住时间=%lld 退房时间=%lld 费用=%.2f\n",
               order_get_id(o),
               order_get_guest_prc_citizen_id(o),
               order_get_room_id(o),
               order_status_to_string(order_get_status(o)),
               (int)order_get_status(o),
               (long long)order_get_actual_checkin_time(o),
               (long long)order_get_actual_checkout_time(o),
               order_get_cost(o));
    }
}

static void print_guest_registry(const GuestRegistry* registry, const char* title) {
    size_t i, n = 0;
    if (registry == NULL) return;
    n = guest_registry_size(registry);
    printf("-- %s（数量=%zu） --\n", title, n);
    for (i = 0; i < n; ++i) {
        Guest* g = guest_registry_get_at(registry, i);
        if (g == NULL) continue;
        printf("住客ID=%d 身份证=%s 姓名=%s 电话=%s 性别=%d 入住次数=%d\n",
               guest_get_id(g),
               guest_get_prc_citizen_id(g),
               guest_get_name(g),
               guest_get_tel(g),
               (int)guest_get_sex(g),
               guest_get_checkin_count(g));
    }
}

static void print_room_registry(const RoomRegistry* registry, const char* title) {
    size_t i, n = 0;
    if (registry == NULL) return;
    n = room_registry_size(registry);
    printf("-- %s（数量=%zu） --\n", title, n);
    for (i = 0; i < n; ++i) {
        Room* r = room_registry_get_at(registry, i);
        if (r == NULL) continue;
        printf("房间ID=%d 房号=%s 楼层=%d 床位=%d 面积=%.2f 房型ID=%d 状态=%s(%d)\n",
               room_get_id(r),
               room_get_no(r),
               room_get_floor(r),
               room_get_bed_count(r),
               room_get_area(r),
               room_get_type_id(r),
               room_status_to_string(room_get_status(r)),
               (int)room_get_status(r));
    }
}

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
    int room_type_ok = room_type_repository_load_all_from_file(c->tr, config_get_room_type_file_path());
    int room_ok = room_repository_load_all_from_file(c->rr, config_get_room_file_path());
    int guest_ok = guest_repository_load_all_from_file(c->gr, config_get_guest_file_path());
    int order_ok = order_repository_load_all_from_file(c->orr, config_get_order_file_path());
    int price_ok = price_repository_load_all_from_file(c->ph, config_get_price_file_path());

    if (!room_type_ok) LOG_ERROR("load room_type failed: path='%s'", config_get_room_type_file_path());
    if (!room_ok) LOG_ERROR("load room failed: path='%s'", config_get_room_file_path());
    if (!guest_ok) LOG_ERROR("load guest failed: path='%s'", config_get_guest_file_path());
    if (!order_ok) LOG_ERROR("load order failed: path='%s'", config_get_order_file_path());
    if (!price_ok) LOG_ERROR("load price failed: path='%s'", config_get_price_file_path());

    return room_type_ok && room_ok && guest_ok && order_ok && price_ok;
}

static int app_init(AppCtx* c) {
    if (c == NULL) return 0;

    memset(c, 0, sizeof(*c));
    setlocale(LC_ALL, "");

    if (!config_load()) {
        LOG_ERROR("config load failed: path='%s'", config_get_config_file_path());
        return 0;
    }

    price_repository_set_file_path(config_get_price_file_path());

    c->rr = room_registry_create();
    c->tr = room_type_registry_create();
    c->gr = guest_registry_create();
    c->orr = order_registry_create();
    c->ph = price_history_create();

    if (c->rr == NULL || c->tr == NULL || c->gr == NULL || c->orr == NULL || c->ph == NULL) {
        LOG_ERROR("app init failed: registry allocation failed");
        return 0;
    }

    inventory_service_init(&c->inv, c->rr, c->tr);
    guest_profile_service_init(&c->gps, c->gr);
    price_service_init(&c->ps, c->ph);
    stay_service_init(&c->stay, c->orr, c->ph, &c->inv, &c->gps);
    insight_service_init(&c->ins, c->rr, c->gr, c->orr, c->tr);

    if (!load_from_repository(c)) {
        LOG_ERROR("persistent data load has failures, fallback to seed data");
        seed(c);
    }

    return 1;
}

static void do_inventory(AppCtx* c, UiState* ui) {
    int op = in_i("房间管理:1新增房间 2更新房间 3删除房间 4设为预订 5设为入住 6设为空闲 7设为维护 8按房型查空闲 9列出全部房间 10新增房型 11修改房型名 12删除房型 13列出房型");
    ServiceCode rc = SERVICE_OK;
    if (op == 1) {
        int type;
        if (!choose_room_type(c, &type)) { msg("已取消或输入非法"); return; }
        char no[32], desc[64];
        int floor = in_i("floor=");
        int bed = in_i("bed_count=");
        double area = in_d("area=");
        in_s("room_no=", no, 32);
        in_s("description=", desc, 64);
        CreateRoomCmd cmd;
        memset(&cmd, 0, sizeof(cmd));
        snprintf(cmd.room_no, sizeof(cmd.room_no), "%s", no);
        snprintf(cmd.description, sizeof(cmd.description), "%s", desc);
        cmd.floor = floor; cmd.bed_count = bed; cmd.area = area; cmd.type_id = type;
        rc = inventory_service_create_room(&c->inv, &cmd);
        msg_service_result("inventory.create_room", rc);
    } else if (op == 2) {
        int id = in_i("room_id=");
        int type;
        if (!choose_room_type(c, &type)) { msg("已取消或输入非法"); return; }
        char no[32], desc[64];
        int floor = in_i("floor=");
        int bed = in_i("bed_count=");
        double area = in_d("area=");
        in_s("room_no=", no, 32);
        in_s("description=", desc, 64);
        Room* req = room_create(id, no, floor, bed, area, desc, type, ROOM_STATUS_IDLE, time(NULL), time(NULL), 0);
        rc = inventory_service_update_room(&c->inv, req);
        msg_service_result("inventory.update_room", rc);
        if (req != NULL) room_destroy(req);
    } else if (op == 3) { int rid; if (!choose_room_id(c, 0, &rid)) { msg("已取消或输入非法"); return; } rc = inventory_service_remove_room(&c->inv, rid); msg_service_result("inventory.remove_room", rc); }
    else if (op == 4) { int rid; if (!choose_room_id(c, 0, &rid)) { msg("已取消或输入非法"); return; } rc = inventory_service_mark_reserved(&c->inv, rid); msg_service_result("inventory.mark_reserved", rc); }
    else if (op == 5) { int rid; if (!choose_room_id(c, 0, &rid)) { msg("已取消或输入非法"); return; } rc = inventory_service_mark_occupied(&c->inv, rid); msg_service_result("inventory.mark_occupied", rc); }
    else if (op == 6) { int rid; if (!choose_room_id(c, 0, &rid)) { msg("已取消或输入非法"); return; } rc = inventory_service_mark_vacant(&c->inv, rid); msg_service_result("inventory.mark_vacant", rc); }
    else if (op == 7) { int rid; if (!choose_room_id(c, 0, &rid)) { msg("已取消或输入非法"); return; } rc = inventory_service_mark_maintenance(&c->inv, rid); msg_service_result("inventory.mark_maintenance", rc); }
    else if (op == 8) {
        size_t i, n = 0;
        int type;
        if (!choose_room_type(c, &type)) { msg("已取消或输入非法"); return; }
        RoomView views[256];
        rc = inventory_service_list_all_room_views(&c->inv, views, 256, &n);
        msg_service_result("inventory.list_by_type", rc);
        if (rc == SERVICE_OK) {
            size_t cnt = 0;
            for (i = 0; i < n; ++i) {
                if (views[i].type_id == type && views[i].status == ROOM_STATUS_IDLE) {
                    ++cnt;
                    printf("room_no=%s type=%s status=%s(%d)\n", views[i].room_no, views[i].type_name,
                           room_status_to_string(views[i].status), (int)views[i].status);
                }
            }
            printf("可用房间数量=%zu\n", cnt);
        }
    } else if (op == 9) {
        size_t i, n = 0;
        RoomView views[256];
        rc = inventory_service_list_all_room_views(&c->inv, views, 256, &n);
        msg_service_result("inventory.list_all_room_views", rc);
        if (rc == SERVICE_OK) for (i = 0; i < n; ++i) {
            printf("room_no=%s floor=%d bed=%d area=%.2f type=%s(%d) status=%s(%d)\n",
                   views[i].room_no, views[i].floor, views[i].bed_count, views[i].area,
                   views[i].type_name, views[i].type_id,
                   room_status_to_string(views[i].status), (int)views[i].status);
        }
    } else if (op == 10) {
        int type_id = in_i("type_id=");
        char type_name[32];
        in_s("type_name=", type_name, 32);
        rc = inventory_service_add_room_type(&c->inv, room_type_create(type_id, type_name));
        msg_service_result("inventory.add_room_type", rc);
    } else if (op == 11) {
        int type_id;
        if (!choose_room_type(c, &type_id)) { msg("已取消或输入非法"); return; }
        char type_name[32];
        in_s("new_type_name=", type_name, 32);
        rc = inventory_service_update_room_type_name(&c->inv, type_id, type_name);
        msg_service_result("inventory.update_room_type_name", rc);
    } else if (op == 12) {
        int type_id;
        if (!choose_room_type(c, &type_id)) { msg("已取消或输入非法"); return; }
        rc = inventory_service_remove_room_type(&c->inv, type_id);
        msg_service_result("inventory.remove_room_type", rc);
    } else if (op == 13) { print_room_types(c); }
    ui_set_last_result(ui, "inventory", rc);
}

static void do_guest(AppCtx* c, UiState* ui) {
    int op = in_i("住客档案:1新增 2更新 3删除 4查询 5入住次数+1 6列表");
    ServiceCode rc = SERVICE_OK;
    if (op == 1) {
        char cid[24], name[32], tel[24]; in_s("id_card=", cid, 24); in_s("name=", name, 32); in_s("tel=", tel, 24);
        CreateGuestCmd cmd; memset(&cmd, 0, sizeof(cmd));
        snprintf(cmd.id_card, sizeof(cmd.id_card), "%s", cid);
        snprintf(cmd.name, sizeof(cmd.name), "%s", name);
        snprintf(cmd.tel, sizeof(cmd.tel), "%s", tel);
        cmd.sex = SEX_UNKNOWN;
        rc = guest_profile_service_create_guest(&c->gps, &cmd);
        msg_service_result("guest.create_guest", rc);
    } else if (op == 2) {
        int id = in_i("id=");
        char cid[24], name[32], tel[24];
        in_s("id_card=", cid, 24); in_s("name=", name, 32); in_s("tel=", tel, 24);
        Guest* g = guest_create(id, cid, name, tel, SEX_UNKNOWN, 0);
        rc = guest_profile_service_update_guest(&c->gps, g);
        msg_service_result("guest.update_guest", rc);
        if (g != NULL) guest_destroy(g);
    } else if (op == 3) { char cid[24]; Guest* g = NULL; if (!choose_guest_id_card(c, cid, sizeof(cid))) { msg("已取消或输入非法"); return; } rc = guest_profile_service_find_by_id_card(&c->gps, cid, &g); if (rc == SERVICE_OK && g != NULL) rc = guest_profile_service_remove_guest(&c->gps, guest_get_id(g)); msg_service_result("guest.remove_guest", rc); }
    else if (op == 4) {
        char cid[24]; Guest* g = NULL; if (!choose_guest_id_card(c, cid, sizeof(cid))) { msg("已取消或输入非法"); return; }
        rc = guest_profile_service_find_by_id_card(&c->gps, cid, &g);
        msg_service_result("guest.find_by_id_card", rc);
    } else if (op == 5) {
        char cid[24]; if (!choose_guest_id_card(c, cid, sizeof(cid))) { msg("已取消或输入非法"); return; }
        rc = guest_profile_service_increment_stay_count(&c->gps, cid);
        msg_service_result("guest.increment_stay_count", rc);
    } else if (op == 6) { print_guests(c); }
    ui_set_last_result(ui, "guest", rc);
}

static void do_stay(AppCtx* c, UiState* ui) {
    int op = in_i("入住流程:1预订 2取消预订 3办理入住 4延住 5换房 6退房结算 7计算晚数 8计算金额 9全部订单 10已预订订单 11已入住订单");
    ServiceCode rc = SERVICE_OK;
    if (op == 1) {
        CreateReservationCmd cmd; memset(&cmd, 0, sizeof(cmd));
        if (!choose_guest_id_card(c, cmd.guest_id_card, sizeof(cmd.guest_id_card))) { msg("已取消或输入非法"); return; }
        if (!choose_room_id(c, 1, &cmd.room_id)) { msg("已取消或输入非法"); return; }
        cmd.reserved_at = time(NULL); cmd.checkin_date = now_day(); cmd.checkout_date = now_day() + 86400;
        rc = stay_service_create_reservation(&c->stay, &cmd);
        msg_service_result("stay.create_reservation", rc);
    } else if (op == 2) { int oid; if (!choose_order_id(c, ORDER_STATUS_RESERVED, &oid)) { msg("已取消或输入非法"); return; } rc = stay_service_cancel_reservation(&c->stay, oid); msg_service_result("stay.cancel_reservation", rc); }
    else if (op == 3) { int oid; if (!choose_order_id(c, ORDER_STATUS_RESERVED, &oid)) { msg("已取消或输入非法"); return; } rc = stay_service_check_in(&c->stay, oid, time(NULL)); msg_service_result("stay.check_in", rc); }
    else if (op == 4) { int oid; if (!choose_order_id(c, ORDER_STATUS_CHECKED_IN, &oid)) { msg("已取消或输入非法"); return; } rc = stay_service_extend_checkout_date(&c->stay, oid, now_day() + 2 * 86400); msg_service_result("stay.extend_checkout_date", rc); }
    else if (op == 5) { int oid, rid; if (!choose_order_id(c, ORDER_STATUS_RESERVED, &oid)) { msg("已取消或输入非法"); return; } if (!choose_room_id(c, 1, &rid)) { msg("已取消或输入非法"); return; } rc = stay_service_change_room(&c->stay, oid, rid); msg_service_result("stay.change_room", rc); }
    else if (op == 6) { int oid; SettlementResult s; if (!choose_order_id(c, ORDER_STATUS_CHECKED_IN, &oid)) { msg("已取消或输入非法"); return; } rc = stay_service_check_out_and_settle(&c->stay, oid, time(NULL)+3600, &s); msg_service_result("stay.check_out_and_settle", rc); }
    else if (op == 7) { int n=0; rc = stay_service_calculate_nights(now_day(), now_day()+86400, &n); msg_service_result("stay.calculate_nights", rc); }
    else if (op == 8) { int tid; double a,u; if (!choose_room_type(c, &tid)) { msg("已取消或输入非法"); return; } rc = stay_service_calculate_amount(&c->stay, tid, in_i("nights="), &a, &u); msg_service_result("stay.calculate_amount", rc); }
    else if (op == 9) { print_orders(c); }
    else if (op == 10 || op == 11) {
        size_t i, n = 0;
        StayOrderView views[256];
        OrderStatus status = (op == 10) ? ORDER_STATUS_RESERVED : ORDER_STATUS_CHECKED_IN;
        rc = stay_service_list_order_views_by_status(&c->stay, status, views, 256, &n);
        msg_service_result("stay.list_order_views_by_status", rc);
        if (rc == SERVICE_OK) {
            for (i = 0; i < n; ++i) {
                printf("order_id=%d guest_id_card=%s room_id=%d status=%s(%d) checkin_at=%lld checkout_at=%lld\n",
                       views[i].order_id, views[i].guest_id_card, views[i].room_id,
                       order_status_to_string(views[i].status), (int)views[i].status,
                       (long long)views[i].checkin_time, (long long)views[i].checkout_time);
            }
        }
    }
    ui_set_last_result(ui, "stay", rc);
}

static void do_insight(AppCtx* c, UiState* ui) {
    int op = in_i("报表洞察:1房间历史 2住客历史 3当日到离店 4入住率快照 5月度VIP前10 6营收报表 7最长空置房 8新老客占比");
    OrderRegistry* o1 = order_registry_create(); OrderRegistry* o2 = order_registry_create(); GuestRegistry* g = guest_registry_create(); RoomRegistry* r = room_registry_create();
    OccupancySnapshot s; double total=0, by[16], rr=0,nr=0; int cnt=0;
    ServiceCode rc = SERVICE_OK;
    if (op == 1) {
        int rid;
        if (!choose_room_id(c, 0, &rid)) { msg("已取消或输入非法"); goto INSIGHT_CLEANUP; }
        rc = insight_service_query_room_history(&c->ins, rid, o1);
        msg_service_result("insight.query_room_history", rc);
        if (rc == SERVICE_OK) print_order_registry(o1, "房间历史订单");
    }
    else if (op == 2) {
        char cid[24];
        if (!choose_guest_id_card(c, cid, sizeof(cid))) { msg("已取消或输入非法"); goto INSIGHT_CLEANUP; }
        rc = insight_service_query_guest_history(&c->ins, cid, o1);
        msg_service_result("insight.query_guest_history", rc);
        if (rc == SERVICE_OK) print_order_registry(o1, "住客历史订单");
    }
    else if (op == 3) {
        rc = insight_service_query_daily_arrivals_departures(&c->ins, now_day(), o1, o2);
        msg_service_result("insight.query_daily_arrivals_departures", rc);
        if (rc == SERVICE_OK) {
            print_order_registry(o1, "当日到店");
            print_order_registry(o2, "当日离店");
        }
    }
    else if (op == 4) {
        int i;
        rc = insight_service_occupancy_snapshot(&c->ins, time(NULL), &s);
        msg_service_result("insight.occupancy_snapshot", rc);
        if (rc == SERVICE_OK) {
            printf("总体入住率=%.4f\n", s.overall_occupancy);
            printf("房型统计数=%d\n", s.by_room_type_count);
            for (i = 0; i < s.by_room_type_count && i < 16; ++i) {
                printf("  房型序号=%d 入住率=%.4f\n", i, s.by_room_type[i]);
            }
        }
    }
    else if (op == 5) {
        int year = in_i("年份=");
        int month = in_i("月份(1-12)=");
        rc = insight_service_top10_vip_by_month(&c->ins, year, month, g);
        msg_service_result("insight.top10_vip_by_month", rc);
        if (rc == SERVICE_OK) print_guest_registry(g, "月度VIP前10");
    }
    else if (op == 6) {
        int days = in_i("报表天数(>=1)=");
        DateRange range;
        int i;
        if (days <= 0) days = 1;
        range.start_inclusive = now_day() - (time_t)(days - 1) * 86400;
        range.end_exclusive = now_day() + 86400;
        rc = insight_service_revenue_report(&c->ins, range, &total, by, &cnt);
        msg_service_result("insight.revenue_report", rc);
        if (rc == SERVICE_OK) {
            printf("区间=[%lld,%lld) 总营收=%.2f 房型数=%d\n", (long long)range.start_inclusive, (long long)range.end_exclusive, total, cnt);
            for (i = 0; i < cnt && i < 16; ++i) {
                printf("  房型序号=%d 营收=%.2f\n", i, by[i]);
            }
        }
    }
    else if (op == 7) {
        rc = insight_service_longest_vacant_rooms(&c->ins, time(NULL), r);
        msg_service_result("insight.longest_vacant_rooms", rc);
        if (rc == SERVICE_OK) print_room_registry(r, "最长空置房间");
    }
    else if (op == 8) {
        int days = in_i("报表天数(>=1)=");
        DateRange range;
        if (days <= 0) days = 1;
        range.start_inclusive = now_day() - (time_t)(days - 1) * 86400;
        range.end_exclusive = now_day() + 86400;
        rc = insight_service_guest_source_ratio(&c->ins, range, &rr, &nr);
        msg_service_result("insight.guest_source_ratio", rc);
        if (rc == SERVICE_OK) {
            printf("区间=[%lld,%lld) 回头客占比=%.4f 新客占比=%.4f\n",
                   (long long)range.start_inclusive, (long long)range.end_exclusive, rr, nr);
        }
    }

INSIGHT_CLEANUP:
    order_registry_destroy(o1);
    order_registry_destroy(o2);
    guest_registry_destroy(g);
    room_registry_destroy(r);

    ui_set_last_result(ui, "insight", rc);
}

static void do_price(AppCtx* c, UiState* ui) {
    int op = in_i("价格管理:1设置市场价 2查询当前价");
    ServiceCode rc = SERVICE_OK;
    if (op == 1) {
        int tid;
        double amount;
        if (!choose_room_type(c, &tid)) { msg("已取消或输入非法"); return; }
        amount = in_d("amount=");
        rc = price_service_set_market_price(&c->ps, tid, amount, time(NULL));
        msg_service_result("price.set_market_price", rc);
    } else if (op == 2) {
        int tid;
        double current = 0.0;
        if (!choose_room_type(c, &tid)) { msg("已取消或输入非法"); return; }
        rc = price_service_get_current_market_price(&c->ps, tid, &current);
        msg_service_result("price.get_current_market_price", rc);
        if (rc == SERVICE_OK) {
            printf("房型ID=%d 当前价格=%.2f\n", tid, current);
        }
    }
    ui_set_last_result(ui, "price", rc);
}

void tui_app_run(void) {
    AppCtx c;
    UiState ui;
    int run = 1;
    if (!app_init(&c)) return;
    memset(&ui, 0, sizeof(ui));
    snprintf(ui.last_result, sizeof(ui.last_result), "系统已就绪");

    while (run) {
        char ch[8] = {0};
        ui_render_main(&ui);
        printf("请选择: ");
        if (!read_line(ch, sizeof(ch))) break;

        if (ch[0] == '1') { snprintf(ui.current_module, sizeof(ui.current_module), "房间管理"); snprintf(ui.current_object, sizeof(ui.current_object), "房间/房型"); do_inventory(&c, &ui); }
        else if (ch[0] == '2') { snprintf(ui.current_module, sizeof(ui.current_module), "住客档案"); snprintf(ui.current_object, sizeof(ui.current_object), "住客信息"); do_guest(&c, &ui); }
        else if (ch[0] == '3') { snprintf(ui.current_module, sizeof(ui.current_module), "入住流程"); snprintf(ui.current_object, sizeof(ui.current_object), "预订/入住订单"); do_stay(&c, &ui); }
        else if (ch[0] == '4') { snprintf(ui.current_module, sizeof(ui.current_module), "价格管理"); snprintf(ui.current_object, sizeof(ui.current_object), "价格维护"); do_price(&c, &ui); }
        else if (ch[0] == '5') { snprintf(ui.current_module, sizeof(ui.current_module), "报表洞察"); snprintf(ui.current_object, sizeof(ui.current_object), "统计报表"); do_insight(&c, &ui); }
        else if (ch[0] == 'q' || ch[0] == 'Q') run = 0;
        else snprintf(ui.last_result, sizeof(ui.last_result), "主菜单非法输入");
    }
}
