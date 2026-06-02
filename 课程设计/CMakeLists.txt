cmake_minimum_required(VERSION 3.10.0)
project(hotel-simple-manage-system VERSION 0.1.0 LANGUAGES C)

find_package(cjson CONFIG REQUIRED)
find_package(libconfig CONFIG REQUIRED)

add_executable(hotel-simple-manage-system
    src/config/config.c
    src/utils/linked_list.c
    src/utils/logger.c
    src/models/room/room.c
    src/models/room/room_type.c
    src/repository/room_repository.c
    src/repository/price_repository.c
    src/repository/guest_repository.c
    src/repository/order_repository.c
    src/service/inventory_service.c
    src/service/guest_profile_service.c
    src/service/price_service.c
    src/service/stay_lifecycle_service.c
    src/service/insight_service.c
    src/ui/tui_app.c
    src/main.c
    src/models/guest/guest.h
    src/models/guest/guest.c
    src/models/order/order.c
    src/models/price/price.c
)

target_include_directories(hotel-simple-manage-system
    PRIVATE
        src
        src/utils
        src/models
        src/repository
)

target_link_libraries(hotel-simple-manage-system
    PRIVATE cjson
    PRIVATE libconfig::config
)

target_compile_features(hotel-simple-manage-system PRIVATE c_std_99)

include(CTest)
enable_testing()

add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
