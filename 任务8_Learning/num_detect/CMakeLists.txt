cmake_minimum_required (VERSION 3.1)

project (lenet_test)

set (CMAKE_BUILD_TYPE Debug)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(src SRC_LIST)

include_directories(include)

find_package(OpenCV REQUIRED)

include_directories("/usr/include/eigen3")

add_executable(main ${SRC_LIST})

link_directories(${OpenCV_LIBRARY_DIRS})

target_link_libraries(main ${OpenCV_LIBS})