CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(httparser)

SET(CMAKE_CXX_FLAGS "-Wextra -Wall -pthread" )
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g" )
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -g" )

SET(HEADERS
    src/httpparser/httprequestparser.h
    src/httpparser/httpresponseparser.h
    src/httpparser/request.h
    src/httpparser/response.h
    src/httpparser/urlparser.h
    tests/common.h
)

SET(SOURCES
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
