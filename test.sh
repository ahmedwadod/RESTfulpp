#!/bin/sh

cmake -Bbuild .
cmake --build build
./build/tests/url
./build/tests/parser
./build/tests/request
./build/tests/router
