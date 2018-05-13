# Minimum CMake required
cmake_minimum_required(VERSION 2.8)

# Project
project(server)

add_subdirectory(xbase)
add_subdirectory(xshare)
add_subdirectory(xdb)
add_subdirectory(test)

add_dependencies(xshare_static xbase_static)
add_dependencies(xdb_static xbase_static)
add_dependencies(test xshare_static xdb_static)
