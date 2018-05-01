cmake_minimum_required(VERSION 3.8)
project(Project3_starter_files)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        header.c
        main.cpp
        Project3.cpp
        String.h)

add_executable(Project3_starter_files ${SOURCE_FILES})