cmake_minimum_required(VERSION 3.1.0)
project(ConfigLoaderLib CXX)

set(CMAKE_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_CXX_STANDARD 20)

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

add_subdirectory(lib/json)

add_library(ConfigLoaderLib STATIC
    src/ConfigLoader.cpp 
    include/ConfigLoader/ConfigLoader.h
)

target_link_libraries(ConfigLoaderLib 
    PRIVATE nlohmann_json::nlohmann_json
)

target_include_directories(ConfigLoaderLib
    PUBLIC include
)
