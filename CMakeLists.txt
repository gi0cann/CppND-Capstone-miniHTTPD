cmake_minimum_required(VERSION 3.7)

project(minihttpd)
add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

# find all source files in src directory
file(GLOB SRC src/*.cpp)

add_executable(minihttpd ${SRC})
