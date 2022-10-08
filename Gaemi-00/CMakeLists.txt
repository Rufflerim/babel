cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
project(Gaemi-00)
add_definitions(-DGEXPORT)


add_subdirectory( math )
add_subdirectory( ecs )
add_subdirectory( input )
add_subdirectory( render )
add_subdirectory( render/sdl )
add_subdirectory( render/vulkan )
add_subdirectory( asset )

file( GLOB Gaemi_SOURCES *.cpp )
if(NOT EMSCRIPTEN)
    add_library( ${PROJECT_NAME} SHARED ${Gaemi_SOURCES})
else()
    add_library( ${PROJECT_NAME} STATIC ${Gaemi_SOURCES})
endif()
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} SDL2 sdl_stb_image ecs input render_sdl render_vulkan asset)

## Define a symbol for dll export
target_compile_definitions(${PROJECT_NAME} PUBLIC GEXPORT=1)

## Debug status
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Gaemi-00 PUBLIC GDEBUG=1)
    target_compile_definitions(render_vulkan PUBLIC GDEBUG=1)
    target_compile_definitions(render_sdl PUBLIC GDEBUG=1)
    target_compile_definitions(math PUBLIC GDEBUG=1)
    target_compile_definitions(ecs PUBLIC GDEBUG=1)
    target_compile_definitions(input PUBLIC GDEBUG=1)
    target_compile_definitions(asset PUBLIC GDEBUG=1)
endif()

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
