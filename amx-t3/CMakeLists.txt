# CMakeList.txt for working as a CLion project

cmake_minimum_required(VERSION 3.10)
project(amx-t3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_COMPILER icpc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES
        src/main.cpp
        src/AmxCalculator.cpp
        src/Matrix.cpp
)

add_executable(amx-t3 ${SOURCE_FILES})
