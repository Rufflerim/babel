cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
project(Gaemi-00)
add_definitions(-DGEXPORT)

add_subdirectory( math )
add_subdirectory( ecs )
add_subdirectory( input )
add_subdirectory( render/sdl )
add_subdirectory( render/vulkan )
add_subdirectory( asset )

file( GLOB Gaemi_SOURCES *.cpp )
if(NOT EMSCRIPTEN)
    add_library( ${PROJECT_NAME} SHARED ${Gaemi_SOURCES})
else()
    add_library( ${PROJECT_NAME} STATIC ${Gaemi_SOURCES} render/RenderRequest.h)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} SDL2 sdl_stb_image ecs input render_sdl render_vulkan asset)

## Define a symbol for dll export
target_compile_definitions(${PROJECT_NAME} PUBLIC GEXPORT=1)

## Post build DLL copy
if(EMSCRIPTEN)
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/Externals/emsdk-3.1.21/game.html
            ${CMAKE_CURRENT_BINARY_DIR}/../
            COMMENT "---- Copy HTML page"
    )
else()
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/Gaemi-00.dll
            ${CMAKE_CURRENT_BINARY_DIR}/../
            COMMENT "---- Copy Engine DLL"
    )
endif()
