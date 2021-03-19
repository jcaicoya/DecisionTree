cmake_minimum_required(VERSION 3.10.2)

project(dectree CXX)

include_directories(../dectreelib)
link_directories(${MY_LIB_OUTPUT_DIRECTORY})

file(GLOB SOURCES *.h *.cpp)
add_executable(dectree ${SOURCES})
target_link_libraries(dectree dectreelib pthread)

message("myapp output directory: ${MY_BIN_OUTPUT_DIRECTORY}")
set_target_properties(dectree
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIRECTORY}")
