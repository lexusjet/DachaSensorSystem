cmake_minimum_required(VERSION 3.0.0)
project(DACHA_PROJECT CXX)

set(CMAKE_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_CXX_STANDARD 20)
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
include_directories(.)

add_subdirectory(Socket_class)
add_subdirectory(SensorMessage)
add_subdirectory(Server)
add_subdirectory(TestClient)
add_subdirectory(test)
add_subdirectory(DatabaseConnector)
add_subdirectory(ServerListener)
add_subdirectory(DatabaseClient)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE ServerLib
    PRIVATE SensorMessageLib
    PRIVATE DatabaseConnectorLib
    PRIVATE ServerListenerLib
    PRIVATE DatabaseClientLib
)
