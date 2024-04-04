CMAKE_MINIMUM_REQUIRED(VERSION 3.25 FATAL_ERROR)
PROJECT(fleet-http-client C CXX)

IF (TARGET fleet-http-client-static)
    RETURN()
ENDIF ()

SET(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
SET(CMAKE_INSTALL_RPATH "$ORIGIN")
SET(CMAKE_CXX_STANDARD 20)
SET(FLEET_HTTP_CLIENT_VERSION 1.2.2)

INCLUDE(CheckPIESupported)
CHECK_PIE_SUPPORTED()
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

OPTION(BRINGAUTO_TESTS "Enable tests" OFF)
OPTION(BRINGAUTO_PACKAGE "Package creation" OFF)
OPTION(BRINGAUTO_INSTALL "Enable install" OFF)
OPTION(BRINGAUTO_SYSTEM_DEP "Enable system dependencies" OFF)
OPTION(BRINGAUTO_SAMPLES "Enable build of sample app, not used in project" OFF)

IF(BRINGAUTO_PACKAGE)
    IF(NOT BRINGAUTO_INSTALL)
        SET(BRINGAUTO_INSTALL ON CACHE BOOL "Forced install due to BRINGAUTO_PACKAGE=ON" FORCE)
        MESSAGE(WARNING "BRINGAUTO_INSTALL is switched to on because of BRINGAUTO_PACKAGE=ON")
    ENDIF()
ENDIF()

FIND_PACKAGE(CMLIB
        COMPONENTS CMDEF CMUTIL STORAGE
        REQUIRED
        )

IF(NOT BRINGAUTO_SYSTEM_DEP)
    INCLUDE(cmake/Dependencies.cmake)
ENDIF()

FIND_PACKAGE(libbringauto_logger 1.2.0 REQUIRED)
FIND_PACKAGE(fleet-protocol-interface 2.0.0 REQUIRED)
FIND_PACKAGE(fleet-protocol-cxx-helpers-static 1.1.1 REQUIRED)

SET(BUILD_SHARED_LIBS OFF)
ADD_SUBDIRECTORY("lib/bringauto-fleet-http-client-generated")

IF(BRINGAUTO_SAMPLES)
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/example/)
ENDIF()

FILE(GLOB_RECURSE source_files "src/*")

CMDEF_ADD_LIBRARY(
        LIBRARY_GROUP fleet-http-client
        TYPE SHARED
        SOURCES ${source_files}
        INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include/
        INSTALL_INCLUDE_DIRECTORIES "include/"
        VERSION ${FLEET_HTTP_CLIENT_VERSION}
)

TARGET_LINK_LIBRARIES(fleet-http-client-shared
        PUBLIC
        fleet-protocol-interface::common-headers-interface
        fleet-protocol-cxx-helpers-static::fleet-protocol-cxx-helpers-static
        bringauto-fleet-http-client-generated-shared
        bringauto_logger::bringauto_logger
)

IF(BRINGAUTO_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/test/)
ENDIF()

IF (BRINGAUTO_INSTALL)
    CMDEF_INSTALL(
            TARGET fleet-http-client-shared
            NAMESPACE fleet-http-client-shared::
    )
    BA_PACKAGE_DEPS_IMPORTED(fleet-http-client-shared)
ENDIF ()

IF (BRINGAUTO_PACKAGE)
    CMDEF_PACKAGE(
            MAIN_TARGET fleet-http-client-shared
            VERSION ${FLEET_HTTP_CLIENT_VERSION}
    )
    SET(CPACK_GENERATOR ZIP)
    SET(CPACK_PACKAGE_CONTACT "BringAuto s.r.o. <maintainers@bringauto.com>")
    INCLUDE(CPack)
ENDIF ()