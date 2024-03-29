cmake_minimum_required(VERSION 3.20)
project(Babel)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Vulkan
set(Vulkan_LIBRARY $ENV{VULKAN_SDK}/Lib/vulkan-1.lib)
set(Vulkan_INCLUDE_DIR $ENV{VULKAN_SDK}/Include)
find_package(Vulkan REQUIRED)

## Executable
add_executable(Babel Babel/Main.cpp)
set_property(TARGET Babel PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Babel>")
target_link_libraries(Babel Babel-Game Gaemi-00)

## Env variables
if(EMSCRIPTEN)
    if(WIN32)
        add_custom_command(TARGET Babel PRE_BUILD
                COMMAND cmd /C "${CMAKE_SOURCE_DIR}/Externals/emsdk-3.1.21/emsdk_env.bat")
    endif()
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC __EMSCRIPTEN__=1)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "-O0 -s NO_DISABLE_EXCEPTION_CATCHING \
        -s INITIAL_MEMORY=67108864 -s ASSERTIONS \
        -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s SDL2_IMAGE_FORMATS=\"['png']\" \
        --preload-file Assets --preload-file Assets/data")
endif()

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC USE_SDL=2)
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC USE_SDL_MIXER=2)
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC USE_WEBGL2=1)

target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
        -Wall -Wextra -Wpedantic -Werror -Werror=uninitialized

        # SDL compatibility
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-reserved-id-macro
        -Wno-c11-extensions
        -Wno-reserved-id-macro

        # Optimization
        #$<$<CONFIG:DEBUG>:-Og>
        #$<$<CONFIG:RELEASE>:-Os>
)

## Build
add_subdirectory( Externals )
add_subdirectory( Gaemi-00 )
add_subdirectory( Babel )

## Debug status and optimisation flag
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC DEBUG=1)
    target_compile_definitions(Gaemi-00 PUBLIC DEBUG=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC DEBUG=0)
endif()

## Vulkan validator
find_program(GLSL_VALIDATOR glslc HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

## Find all the shader files under the shaders folder
file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/Assets/shaders/*.frag"
        "${PROJECT_SOURCE_DIR}/Assets/shaders/*.vert"
        "${PROJECT_SOURCE_DIR}/Assets/shaders/*.comp")

## Iterate each shader
message(STATUS "BUILDING SHADER")
foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/Assets/shaders/${FILE_NAME}.spv")
    message("---- ${GLSL}")
    ##execute glslang command to compile that specific shader
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSL_VALIDATOR} ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)