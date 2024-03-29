cmake_minimum_required(VERSION 3.15)
project(RESTfulpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# --------------------- EXTERNAL LIBRARIES -------------------------------
include(FetchContent)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

FetchContent_Declare(
	llhttp 
	URL "https://github.com/nodejs/llhttp/archive/refs/tags/release/v8.1.0.tar.gz"
)
FetchContent_Declare(
	sockpp 
	GIT_REPOSITORY https://github.com/fpagliughi/sockpp.git
	GIT_TAG v1.0.0
)
# nlohmann is a dependancy that's why we let the user install it
find_package(nlohmann_json REQUIRED)
					
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "")
set(SOCKPP_BUILD_SHARED OFF CACHE INTERNAL "Build shared library")
set(SOCKPP_BUILD_STATIC ON CACHE INTERNAL "Build static library")
FetchContent_MakeAvailable(llhttp sockpp)
# --------------------- END OF EXTERNAL LIBRARIES ------------------------


# --------------------- CREATE TARGET RESTFULPP --------------------------
add_library(RESTfulpp src/Types.cpp src/Request.cpp src/Response.cpp src/Parser.cpp src/Server.cpp src/Client.cpp src/Router.cpp)
target_link_libraries(RESTfulpp sockpp-static llhttp_static nlohmann_json::nlohmann_json)
target_include_directories(RESTfulpp PUBLIC include)
target_include_directories(RESTfulpp PUBLIC ${llhttp_SOURCE_DIR}/include)
target_include_directories(RESTfulpp PUBLIC ${sockpp_SOURCE_DIR}/include)
# --------------------- END OF CREATE TARGET RESTFULPP -------------------

# ------------------------------ OPTIONS ---------------------------------
option(RESTFULPP_BUILD_EXAMPLES "RESTFULPP_BUILD_EXAMPLES" OFF)
option(RESTFULPP_BUILD_TESTS "RESTFULPP_BUILD_TESTS" OFF)

if(RESTFULPP_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

if(RESTFULPP_BUILD_TESTS)
	add_subdirectory(tests)
endif()
# --------------------------- END OF OPTIONS ----------------------------
