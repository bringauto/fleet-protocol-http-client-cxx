ADD_EXECUTABLE(fleet_http_client_sample_app ${CMAKE_CURRENT_LIST_DIR}/main.cpp ${CMAKE_CURRENT_LIST_DIR}/memory_management.cpp)

TARGET_LINK_LIBRARIES(fleet_http_client_sample_app PUBLIC fleet-http-client-shared)