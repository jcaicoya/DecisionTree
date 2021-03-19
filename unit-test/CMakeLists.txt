cmake_minimum_required(VERSION 3.10.2)

project(unit-test CXX)

include_directories(../dectreelib)
link_directories(${MY_LIB_OUTPUT_DIRECTORY})

file(GLOB SOURCES *.h *.cpp)
add_executable(unit-test ${SOURCES})
add_test(NAME unit-test COMMAND unit-test)
target_link_libraries(unit-test dectreelib gtest pthread)

message("mytest output directory: ${MY_BIN_OUTPUT_DIRECTORY}")
set_target_properties(unit-test
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIRECTORY}")
