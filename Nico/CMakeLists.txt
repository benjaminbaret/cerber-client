cmake_minimum_required(VERSION 3.10)
project(DBus-cerber C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GIO_UNIX REQUIRED gio-unix-2.0)

set(CMAKE_C_STANDARD 11)

# Add the path to the directory containing dbus.h
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(DBus-cerber main.c DBusLib.c)

target_include_directories(DBus-cerber PRIVATE ${GIO_INCLUDE_DIRS} ${GIO_UNIX_INCLUDE_DIRS} )
target_link_libraries(DBus-cerber PRIVATE ${GIO_LIBRARIES} ${GIO_UNIX_LIBRARIES})


install(TARGETS DBus-cerber DESTINATION bin)