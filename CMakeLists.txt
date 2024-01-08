cmake_minimum_required(VERSION 3.10)
project(RESTfulpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory("lib/sockpp-1.0.0")
add_subdirectory("lib/httpparser")

add_library(RESTfulpp src/Server.cpp)
target_link_libraries(RESTfulpp sockpp-static)

target_include_directories(RESTfulpp PUBLIC  include)
target_include_directories(RESTfulpp PUBLIC  lib/httpparser/src)
target_include_directories(RESTfulpp PUBLIC  lib/sockpp-1.0.0/include)

set(RESTFULPP_BUILD_EXAMPLE True)

if(RESTFULPP_BUILD_EXAMPLE)
	add_subdirectory(examples)
endif()
