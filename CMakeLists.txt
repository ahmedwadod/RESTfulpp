cmake_minimum_required(VERSION 3.10)
project(RESTfulpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(RESTfulpp src/Server.cpp)
target_link_directories(RESTfulpp PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>)
target_link_libraries(RESTfulpp sockpp)

add_executable(hello examples/hello.cpp)
target_link_libraries(hello RESTfulpp)

target_include_directories(RESTfulpp PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

