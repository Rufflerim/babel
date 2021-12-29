cmake_minimum_required(VERSION 3.20)
project(DumbKnights-Game)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory( components )
add_subdirectory( scene )

file( GLOB Game_SOURCES *.cpp )
add_library( ${PROJECT_NAME} ${Game_SOURCES} )
target_include_directories(${PROJECT_NAME} PUBLIC Gaemi-00)
target_link_libraries(${PROJECT_NAME} Gaemi-00 SDL2 scene comp)

## Define a symbol for dll import
target_compile_definitions(${PROJECT_NAME} PUBLIC GEXPORT=0)

## Post build assets copy
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_BINARY_DIR}/../../Assets
        $<TARGET_FILE_DIR:DumbKnights>/Assets
        COMMENT "---- Copy Assets"
)

