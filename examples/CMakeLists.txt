cmake_minimum_required(VERSION 3.10)

add_executable(hello hello.cpp)
target_link_directories(hello PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(hello PRIVATE RESTfulpp)

add_executable(hello_json hello_json.cpp)
target_link_directories(hello_json PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(hello_json PRIVATE RESTfulpp)
