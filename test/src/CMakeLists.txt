cmake_minimum_required(VERSION 3.10)
project(test-unitaires)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GIO_UNIX REQUIRED gio-unix-2.0)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(DBUS REQUIRED dbus-1)
pkg_search_module(JSONGLIB REQUIRED json-glib-1.0)


file(GLOB SOURCES_TEST "*.c")

add_executable(test-unitaires ${SOURCES_TEST})

target_include_directories(test-unitaires PRIVATE 
    ${GIO_INCLUDE_DIRS}
    ${GIO_UNIX_INCLUDE_DIRS}
    ${DBUS_INCLUDE_DIRS} 
    ${CURL_INCLUDE_DIRS}
    ${JSONGLIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# Lien avec la bibliothèque partagée "cerber-client-lib"
target_link_libraries(test-unitaires PRIVATE 
    ${GIO_LIBRARIES}
    ${GIO_UNIX_LIBRARIES}
    ${DBUS_LIBRARIES}
    ${CURL_LIBRARIES}
    ${JSONGLIB_LIBRARIES}
    cerber-client-lib
    ${CHECK_LIBRARIES}
    check
    m
    rt
    pthread
    subunit
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR} /../../build/bin/)
