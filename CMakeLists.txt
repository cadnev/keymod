cmake_minimum_required(VERSION 3.24)

project(keymod)

set(CMAKE_CXX_STANDARD 17)

SET(PROJ_TARGET keymod)
SET (PROJ_TARGET_LIB hooks)

include_directories(include)

set(SOURCE_LIST src/main.cpp
        src/replace.cpp
        src/configure.cpp)

set(SOURCE_LIST_LIB src/scan_keys.cpp
        src/replace.cpp
        src/daemon_hooks.cpp)

add_link_options(-static)

add_library(${PROJ_TARGET_LIB} SHARED ${SOURCE_LIST_LIB})
add_executable(${PROJ_TARGET} ${SOURCE_LIST})

set_target_properties(${PROJ_TARGET_LIB} PROPERTIES PREFIX "")

