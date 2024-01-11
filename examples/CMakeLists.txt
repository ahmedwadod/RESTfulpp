cmake_minimum_required(VERSION 3.10)

add_executable(hello hello.cpp)
target_link_directories(hello PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(hello PRIVATE RESTfulpp)

add_executable(hello_client hello_client.cpp)
target_link_directories(hello_client PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(hello_client PRIVATE RESTfulpp)
