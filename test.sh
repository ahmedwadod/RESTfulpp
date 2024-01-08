#!/bin/sh

cmake -Bbuild .
cmake --build build
./build/tests/url
./build/tests/request
