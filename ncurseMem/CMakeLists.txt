cmake_minimum_required(VERSION 3.10)
project(ncurseMem)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(ncurseMem
        memNode.h
        memory.cpp
        simulator.h)
