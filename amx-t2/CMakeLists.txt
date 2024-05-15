cmake_minimum_required(VERSION 3.10)
project(amx-t2 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_COMPILER icpc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(SOURCE_FILES
        src/main.cpp
        src/AmxCalculator.cpp
)

add_executable(amx-t2 ${SOURCE_FILES})
