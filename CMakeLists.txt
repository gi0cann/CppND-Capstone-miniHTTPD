cmake_minimum_required(VERSION 3.7)

project(minihttpd)

# find all source files in src directory
file(GLOB SRC src/*.cpp)

# create directory with html page
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/htdocs)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/htdocs/index.html "<html><body><h1>Welcome to MiniHTTPD!</h1></body></html>")

add_executable(minihttpd ${SRC})

if(WIN32)
    target_link_libraries(minihttpd wsock32 ws2_32)
else()
    add_definitions(-std=c++17)
    add_definitions(-pthread)

    set(CXX_FLAGS "-Wall -pthread")
    set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")
endif()

