cmake_minimum_required (VERSION 3.1)

project (serial_test)

set (CMAKE_BUILD_TYPE Debug)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(src SRC_LIST)

add_executable(main ${SRC_LIST})