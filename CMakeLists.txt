cmake_minimum_required(VERSION 3.15)

project("interpolation" CXX)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

find_package(Boost 1.7 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
