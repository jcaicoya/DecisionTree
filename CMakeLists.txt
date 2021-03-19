cmake_minimum_required(VERSION 3.10.2)

project(dectree)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_BUILD_TYPE Debug)
message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode")

set(CMAKE_CXX_STANDARD 17)

set(MY_LIB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(MY_BIN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
message("my lib output directory is: ${MY_LIB_OUTPUT_DIRECTORY}")
message("my bin output directory is: ${MY_BIN_OUTPUT_DIRECTORY}")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dectreelib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/unit-test)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dectree)

add_dependencies(unit-test dectreelib)
add_dependencies(dectree dectreelib)
