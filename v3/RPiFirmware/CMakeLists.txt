cmake_minimum_required(VERSION 3.7)
project(RPiFirmware)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(RPiFirmware ${SOURCE_FILES})