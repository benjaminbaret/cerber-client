cmake_minimum_required(VERSION 3.10)
project(cerber-client C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GIO_UNIX REQUIRED gio-unix-2.0)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(DBUS REQUIRED dbus-1)

set(CMAKE_C_STANDARD 11)

add_executable(cerber-client main.c rauc-installer-generated.c)

target_include_directories(cerber-client PRIVATE ${GIO_INCLUDE_DIRS} ${GIO_UNIX_INCLUDE_DIRS} ${DBUS_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})
target_link_libraries(cerber-client PRIVATE ${GIO_LIBRARIES} ${GIO_UNIX_LIBRARIES} ${DBUS_LIBRARIES} ${CURL_LIBRARIES})


install(TARGETS cerber-client DESTINATION bin)