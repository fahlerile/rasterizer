cmake_minimum_required(VERSION 3.18.4)
project(rasterizer)

set(EXECUTABLE_NAME rasterizer)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

add_subdirectory(src)
add_subdirectory(lib)

file(COPY res/ DESTINATION ${CMAKE_BINARY_DIR}/bin/res/)

