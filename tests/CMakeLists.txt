cmake_minimum_required(VERSION 3.10)

find_package(Catch2 3 REQUIRED)

add_executable(url url.cpp)
target_link_directories(url PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(url PRIVATE Catch2::Catch2WithMain PRIVATE RESTfulpp)

add_executable(parser parser.cpp)
target_link_directories(parser PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(parser PRIVATE Catch2::Catch2WithMain PRIVATE RESTfulpp)

add_executable(request request.cpp)
target_link_directories(request PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(request PRIVATE Catch2::Catch2WithMain PRIVATE RESTfulpp)

add_executable(router router.cpp)
target_link_directories(router PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(router PRIVATE Catch2::Catch2WithMain PRIVATE RESTfulpp)

include(CTest)
include(Catch)
catch_discover_tests(url)
catch_discover_tests(parser)
catch_discover_tests(request)
catch_discover_tests(router)
