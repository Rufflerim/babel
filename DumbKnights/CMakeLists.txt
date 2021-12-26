cmake_minimum_required(VERSION 3.20)
project(DumbKnights-Game)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory( components )
add_subdirectory( scene )

file( GLOB Game_SOURCES *.cpp )
add_library( ${PROJECT_NAME} ${Game_SOURCES} )
target_include_directories(${PROJECT_NAME} PUBLIC Gaemi-00)
target_link_libraries(${PROJECT_NAME} Gaemi-00 SDL2 scene comp)
