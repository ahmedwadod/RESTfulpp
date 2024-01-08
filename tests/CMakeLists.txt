cmake_minimum_required(VERSION 3.10)

find_package(Catch2 3 REQUIRED)

add_executable(url url.cpp)
target_link_directories(hello PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(url PRIVATE Catch2::Catch2WithMain PRIVATE RESTfulpp)

include(CTest)
include(Catch)
catch_discover_tests(url)
