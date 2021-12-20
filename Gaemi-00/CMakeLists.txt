cmake_minimum_required(VERSION 3.20)

project(Gaemi-00)
set(CMAKE_CXX_STANDARD 17)

file( GLOB Gaemi_SOURCES *.cpp )
add_library( ${PROJECT_NAME} SHARED ${Gaemi_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} SDL2)

## Define a symbol for dll export, cf. Gaemi.h
target_compile_definitions(${PROJECT_NAME} PRIVATE GAEMI00_EXPORTS=1)

