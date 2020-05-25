cmake_minimum_required(VERSION 3.7)

project(minihttpd)

# find all source files in src directory
file(GLOB SRC src/*.cpp)

add_executable(minihttpd ${SRC})

if(WIN32)
    target_link_libraries(minihttpd wsock32 ws2_32)
else()
    add_definitions(-std=c++17)

    set(CXX_FLAGS "-Wall")
    set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")
endif()

