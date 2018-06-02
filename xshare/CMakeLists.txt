# Minimum CMake required
cmake_minimum_required(VERSION 2.8)

# Project
project(xshare)

message("------------ Project -------------")
message("  Project name: " ${PROJECT_NAME})

if (CMAKE_BUILD_TYPE)
else()
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

message("------------ Options -------------")
message("  CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# src list
aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/net SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/net/http SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/work SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/config SRC_LIST)
include_directories(${PROJECT_SOURCE_DIR}/..)

message("  SRC_LIST: " ${SRC_LIST})

if (UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -Wall -pthread")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -ggdb -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -ggdb -DNDEBUG")
    add_definitions(-DEVPP_HTTP_CLIENT_SUPPORTS_SSL)
endif()

message("------------ Cxx flags -------------")
message("  CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")
message("------------------------------------")

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../bin/lib)

message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message(STATUS "CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS})
#message(STATUS "EXECUTABLE_OUTPUT_PATH " ${EXECUTABLE_OUTPUT_PATH})
message(STATUS "LIBRARY_OUTPUT_PATH " ${LIBRARY_OUTPUT_PATH})

set(TARGET "xshare")
set(STATIC_T "xshare_static")

add_library("${STATIC_T}" STATIC ${SRC_LIST})
target_link_libraries("${STATIC_T}" xbase)
set_target_properties("${STATIC_T}" PROPERTIES OUTPUT_NAME "${TARGET}")
