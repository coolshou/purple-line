cmake_minimum_required (VERSION 2.8)

set(PROJECT_NAME "purpleline")
set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "3")

project(${PROJECT_NAME})

set(CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/cmake 
    ${CMAKE_MODULE_PATH}
    )

find_package(PkgConfig REQUIRED)

add_definitions(-shared -I/usr/include/thrift -Wl,-z,defs -Wall -fPIC -DHAVE_INTTYPES_H -DHAVE_CONFIG_H -DPURPLE_PLUGINS -std=c++1y)

pkg_check_modules(GLIB REQUIRED  glib-2.0)
pkg_check_modules(PURPLE REQUIRED purple)

include_directories(
    ${PURPLE_INCLUDE_DIRS}
    ${GLIB2_INCLUDE_DIRS}
    )

set(SRC_LIST
    thrift_line/line_main_constants.cpp
    thrift_line/line_main_types.cpp
    thrift_line/TalkService.cpp
    pluginmain.cpp
    linehttptransport.cpp
    thriftclient.cpp
    httpclient.cpp
    purpleline.cpp
    purpleline_login.cpp
    purpleline_blist.cpp
    purpleline_chats.cpp
    poller.cpp
    pinverifier.cpp
    )

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
    ${PURPLE_LIBRARIES}
    ${GLIB2_LIBRARIES}
    thrift
    )

exec_program("pkg-config --variable=plugindir purple 2>/dev/null"
        OUTPUT_VARIABLE LIB_INSTALL_DIR)

install(TARGETS ${PROJECT_NAME} DESTINATION ${LIB_INSTALL_DIR})
install(DIRECTORY "pixmaps/"
    DESTINATION "${PURPLE_DATADIR}pixmaps/pidgin/protocols/"
    )

# package settings
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "LINE instant messenger protocol for libpurple")
set(CPACK_PACKAGE_VENDOR "Matti Virkkunen")
set(CPACK_PACKAGE_DESCRIPTION "purple-line is a LINE protocol plugin for libpurple")
set(CPACK_PACKAGE_CONTACT "edhelas@movim.eu")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) 
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${VERSION}")
set(ACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${VERSION}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpurple0 (>= 2.10.11), libglib2.0-0 (>= 2.31.8), thrift (>= 1.0.0-dev)")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jaussoin Timothée") #required

set(PACK "DEB" CACHE STRING "Generate a Package")
set(CPACK_GENERATOR ${PACK})

include(CPack)
