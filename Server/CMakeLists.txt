cmake_minimum_required(VERSION 3.0.0)
project(ServerLib CXX)

set(CMAKE_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_CXX_STANDARD 20)


add_library(ServerLib STATIC
    Server.h
    Server.cpp
)

target_include_directories(ServerLib PUBLIC .)