cmake_minimum_required(VERSION 3.20)
project(DumbKnights)

## Executable
add_executable(${CMAKE_PROJECT_NAME} DumbKnights/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} DumbKnights-Game Gaemi-00)

## Setup compilation
set(CMAKE_CXX_STANDARD 17)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC DEBUG=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC DEBUG=0)
endif()

## Build
add_subdirectory( Externals )
add_subdirectory( Gaemi-00 )
add_subdirectory( DumbKnights )
