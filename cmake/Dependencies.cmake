SET(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH FALSE)

BA_PACKAGE_LIBRARY(zlib          v1.2.11)
BA_PACKAGE_LIBRARY(ba-logger     v1.2.0)

IF (BRINGAUTO_TESTS)
    BA_PACKAGE_LIBRARY(gtest         v1.12.1)
ENDIF ()