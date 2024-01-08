cmake_minimum_required(VERSION 3.10)

add_executable(hello hello.cpp)
target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/lib/httpparser/src)
target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/lib/sockpp-1.0.0/include)
target_link_directories(hello PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(hello PRIVATE RESTfulpp)
