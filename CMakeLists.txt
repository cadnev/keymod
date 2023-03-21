cmake_minimum_required(VERSION 3.24)

project(keymod)

set(CMAKE_CXX_STANDARD 17)

set(PROJ_TARGET keymod)
set(PROJ_TARGET_LIB hooks)
set(LOGGER_PATH include/logger)

include_directories(include)
include_directories(${LOGGER_PATH}/include)

set(SOURCE_LIST src/main.cpp
        src/configure.cpp)

set(SOURCE_LIST_LIB src/scan_keys.cpp
        src/daemon_hooks.cpp)

add_link_options(-static)

add_library(${PROJ_TARGET_LIB} SHARED ${SOURCE_LIST_LIB})
add_executable(${PROJ_TARGET} ${SOURCE_LIST})

set_target_properties(${PROJ_TARGET_LIB} PROPERTIES PREFIX "")

