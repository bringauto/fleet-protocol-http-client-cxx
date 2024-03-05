CMAKE_MINIMUM_REQUIRED(VERSION 3.25)
PROJECT(fleet-http-client)

SET(CMAKE_CXX_STANDARD 20)

FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})
FILE(GLOB_RECURSE source_test_files "${CMAKE_CURRENT_LIST_DIR}/src/*")
ADD_EXECUTABLE(fleet-http-client-tests ${source_test_files} ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
TARGET_INCLUDE_DIRECTORIES(fleet-http-client-tests PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/")
TARGET_COMPILE_OPTIONS(fleet-http-client-tests PRIVATE -Wall -Wextra -Wpedantic -fsanitize=address)
TARGET_LINK_LIBRARIES(fleet-http-client-tests PUBLIC ${GTEST_LIBRARIES} bringautolib -fsanitize=address)
INCLUDE(GoogleTest)
GTEST_DISCOVER_TESTS(fleet-http-client-tests)