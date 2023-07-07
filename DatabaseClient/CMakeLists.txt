cmake_minimum_required(VERSION 3.0.0)
project(DatabaseClient CXX)

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
# include_directories(${Boost_INCLUDE_DIRS})

add_library(DatabaseClientLib STATIC
    src/DatabaseClient.cpp
    include/DatabaseClient/DatabaseClient.h
)

target_link_libraries(DatabaseClientLib 
    PUBLIC SensorMessageLib
    PUBLIC DatabaseConnectorLib
)

target_include_directories(DatabaseClientLib
    PUBLIC include
)