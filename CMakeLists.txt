cmake_minimum_required(VERSION 3.20)
project(DumbKnights)

## Executable
add_executable(DumbKnights DumbKnights/Main.cpp)
set_property(TARGET DumbKnights PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Gaemi>")
target_link_libraries(DumbKnights DumbKnights-Game Gaemi-00)

## Setup compilation
set(CMAKE_CXX_STANDARD 17)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC DEBUG=1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC DEBUG=0)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

## Build
add_subdirectory( Externals )
add_subdirectory( Gaemi-00 )
add_subdirectory( DumbKnights )

