cmake_minimum_required(VERSION "3.22.1")
project(API)

# Find required packages
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(JSONGLIB REQUIRED json-glib-1.0)

# Include directories for the required packages
include_directories(${CURL_INCLUDE_DIRS})
include_directories(${JSONGLIB_INCLUDE_DIRS})

# Add your source file
add_executable(my_program mainPatch1.c)

# Link libraries
target_link_libraries(my_program PRIVATE ${CURL_LIBRARIES} ${JSONGLIB_LIBRARIES})
target_include_directories(my_program PUBLIC ${JSON_GLIB_INCLUDE_DIRS})
