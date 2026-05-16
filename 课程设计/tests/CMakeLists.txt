add_executable(linked_list_tests
    linked_list_test.c
    ../src/utils/linked_list.c
)

target_include_directories(linked_list_tests
    PRIVATE ../src/utils
)

target_compile_features(linked_list_tests PRIVATE c_std_23)

add_test(NAME linked_list_tests COMMAND linked_list_tests)

add_executable(guest_model_tests
    guest_model_test.c
    ../src/models/guest/guest.c
    ../src/utils/linked_list.c
)
add_test(NAME guest_model_tests COMMAND guest_model_tests)

add_executable(order_model_tests
    order_model_test.c
    ../src/models/order/order.c
    ../src/models/price/price.c
    ../src/utils/linked_list.c
)
add_test(NAME order_model_tests COMMAND order_model_tests)

add_executable(price_model_tests
    price_model_test.c
    ../src/models/price/price.c
    ../src/utils/linked_list.c
)
add_test(NAME price_model_tests COMMAND price_model_tests)

add_executable(room_model_tests
    room_model_test.c
    ../src/models/room/room.c
    ../src/models/room/room_type.c
    ../src/utils/linked_list.c
)
add_test(NAME room_model_tests COMMAND room_model_tests)

add_executable(guest_profile_service_tests
    guest_profile_service_test.c
    ../src/service/guest_profile_service.c
    ../src/models/guest/guest.c
    ../src/utils/linked_list.c
    ../src/utils/logger.c
)
target_include_directories(guest_profile_service_tests PRIVATE ../src)
add_test(NAME guest_profile_service_tests COMMAND guest_profile_service_tests)

add_executable(inventory_service_tests
    inventory_service_test.c
    ../src/service/inventory_service.c
    ../src/models/room/room.c
    ../src/models/room/room_type.c
    ../src/utils/linked_list.c
    ../src/utils/logger.c
)
target_include_directories(inventory_service_tests PRIVATE ../src)
add_test(NAME inventory_service_tests COMMAND inventory_service_tests)

add_executable(stay_lifecycle_service_tests
    stay_lifecycle_service_test.c
    ../src/service/stay_lifecycle_service.c
    ../src/models/order/order.c
    ../src/models/price/price.c
    ../src/models/room/room.c
    ../src/models/room/room_type.c
    ../src/models/guest/guest.c
    ../src/service/inventory_service.c
    ../src/service/guest_profile_service.c
    ../src/repository/order_repository.c
    ../src/repository/price_repository.c
    ../src/repository/room_repository.c
    ../src/repository/guest_repository.c
    ../src/utils/linked_list.c
    ../src/utils/logger.c
)
target_include_directories(stay_lifecycle_service_tests PRIVATE ../src)
target_link_libraries(stay_lifecycle_service_tests PRIVATE cjson)
add_test(NAME stay_lifecycle_service_tests COMMAND stay_lifecycle_service_tests)
