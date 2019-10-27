cmake_minimum_required(VERSION 3.15)

project("interpolation" CXX)

find_package(Boost 1.7 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
