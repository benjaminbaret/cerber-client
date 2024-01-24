# CMakeLists.txt in the "src" directory

cmake_minimum_required(VERSION 3.10)
project(cerber-client)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GIO_UNIX REQUIRED gio-unix-2.0)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(DBUS REQUIRED dbus-1)
pkg_search_module(JSONGLIB REQUIRED json-glib-1.0)

file(GLOB SOURCES "*.c")

# Création d'une bibliothèque partagée (shared library)
add_library(cerber-client-lib SHARED ${SOURCES})

target_include_directories(cerber-client-lib PRIVATE 
    ${GIO_INCLUDE_DIRS}
    ${GIO_UNIX_INCLUDE_DIRS}
    ${DBUS_INCLUDE_DIRS} 
    ${CURL_INCLUDE_DIRS}
    ${JSONGLIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(cerber-client-lib PRIVATE 
    ${GIO_LIBRARIES}
    ${GIO_UNIX_LIBRARIES}
    ${DBUS_LIBRARIES}
    ${CURL_LIBRARIES}
    ${JSONGLIB_LIBRARIES}
)
add_executable(cerber-client ${SOURCES})

target_include_directories(cerber-client PRIVATE 
    ${GIO_INCLUDE_DIRS}
    ${GIO_UNIX_INCLUDE_DIRS}
    ${DBUS_INCLUDE_DIRS} 
    ${CURL_INCLUDE_DIRS}
    ${JSONGLIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(cerber-client PRIVATE 
    ${GIO_LIBRARIES}
    ${GIO_UNIX_LIBRARIES}
    ${DBUS_LIBRARIES}
    ${CURL_LIBRARIES}
    ${JSONGLIB_LIBRARIES}
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../build/bin/)