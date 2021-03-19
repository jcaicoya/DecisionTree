cmake_minimum_required(VERSION 3.10.2)

project(dectreelib CXX)

file(GLOB SOURCES *.h *.cpp)
add_library(dectreelib STATIC ${SOURCES})

set(OUTPUT_LIB_DIR ${CMAKE_BINARY_DIR}/../lib)
message("dectreelib.a output directory: ${MY_LIB_OUTPUT_DIRECTORY}")
set_target_properties(dectreelib 
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${MY_LIB_OUTPUT_DIRECTORY}")
