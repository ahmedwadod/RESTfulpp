cmake_minimum_required(VERSION 3.15)
project(RESTfulpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(FetchContent)

FetchContent_Declare(
	llhttp 
	URL "https://github.com/nodejs/llhttp/archive/refs/tags/release/v8.1.0.tar.gz"
)
FetchContent_Declare(
	sockpp 
	GIT_REPOSITORY https://github.com/fpagliughi/sockpp.git
	GIT_TAG v1.0.0
)
FetchContent_Declare(
	json
	URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
					
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "")
set(SOCKPP_BUILD_SHARED OFF CACHE INTERNAL "Build shared library")
set(SOCKPP_BUILD_STATIC ON CACHE INTERNAL "Build static library")
FetchContent_MakeAvailable(llhttp sockpp json)

add_library(RESTfulpp src/Types.cpp src/Request.cpp src/Response.cpp src/Parser.cpp src/Server.cpp src/Client.cpp src/Router.cpp)
target_link_libraries(RESTfulpp sockpp-static llhttp_static nlohmann_json::nlohmann_json)

target_include_directories(RESTfulpp PUBLIC include)
target_include_directories(RESTfulpp PUBLIC ${llhttp_SOURCE_DIR}/include)
target_include_directories(RESTfulpp PUBLIC ${sockpp_SOURCE_DIR}/include)

# -- Options
set(RESTFULPP_BUILD_EXAMPLE ON)
set(RESTFULPP_BUILD_TESTS ON)

if(RESTFULPP_BUILD_EXAMPLE)
	add_subdirectory(examples)
endif()

if(RESTFULPP_BUILD_TESTS)
	add_subdirectory(tests)
endif()
