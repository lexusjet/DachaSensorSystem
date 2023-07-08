cmake_minimum_required(VERSION 3.0.0)
project(SensorMessageLib CXX)

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

add_library(SensorMessageLib STATIC
    src/SensorMessage.cpp
    include/SensorMessage/SensorMessage.h
)

target_include_directories(SensorMessageLib
    PUBLIC include
)


add_library(SensorMessageValidatorLib STATIC
    src/SensorMessageValidator.cpp
    include/SensorMessageValidator/SensorMessageValidator.h
)
target_link_libraries(SensorMessageValidatorLib
    PUBLIC SensorMessageLib
)
target_include_directories(SensorMessageValidatorLib
    PUBLIC include
)

add_library(SensorMessageConverterLib STATIC
    include/SensorMessageConverter/SensorMessageConverter.h
    src/SensorMessageConverter.cpp 
)

target_link_libraries(SensorMessageConverterLib
    PUBLIC SensorMessageLib
)

target_include_directories(SensorMessageConverterLib
    PUBLIC include
)

