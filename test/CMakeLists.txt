CMAKE_MINIMUM_REQUIRED(VERSION 3.25 FATAL_ERROR)
PROJECT(fleet-http-client C CXX)

SET(CMAKE_CXX_STANDARD 20)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/lib/memory_management/)

FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

FILE(GLOB_RECURSE source_test_files "${CMAKE_CURRENT_LIST_DIR}/src/*")
ADD_EXECUTABLE(fleet-http-client-tests ${source_test_files} ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
TARGET_INCLUDE_DIRECTORIES(fleet-http-client-tests PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/")
TARGET_COMPILE_OPTIONS(fleet-http-client-tests PRIVATE -Wall -Wextra -Wpedantic -fsanitize=address)
TARGET_LINK_OPTIONS(fleet-http-client-tests PRIVATE -fsanitize=address)
TARGET_LINK_LIBRARIES(fleet-http-client-tests PUBLIC
    fleet-http-client-shared
    ${GTEST_LIBRARIES}
    memory_management
)

INCLUDE(GoogleTest)
GTEST_DISCOVER_TESTS(fleet-http-client-tests)