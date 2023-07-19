cmake_minimum_required(VERSION 3.0.0)
project(DelayedQueryManager CXX)

set(CMAKE_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_compile_options(
    
    # -Werror
    -Wall
    -Wno-comment
    -Wno-switch
    
    -Wextra
    -Wno-unused-parameter
    
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    
    # -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    
    # -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    
    # -Wsign-conversion
    -Wsign-promo
    -Wno-unknown-pragmas
)

find_package(Boost 1.74.0 REQUIRED system)
find_package(Threads REQUIRED)

add_library(DelayedQueryManagerLib STATIC
    src/DelayedQueryManager.cpp
    include/DelayedQueryManager/DelayedQueryManager.h
)

target_link_libraries(DelayedQueryManagerLib 
    PRIVATE Threads::Threads
)

target_include_directories(DelayedQueryManagerLib
    PUBLIC include
)