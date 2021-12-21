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

## Copy dll
message("-- Copy external DLLs")
set(BUILD_DIR cmake-build-debug-clang)
## Windows
if(WIN32)
    #### SDL2 dll copy
    if(EXISTS ${PROJECT_SOURCE_DIR}/Externals/SDL2-2.0.16/lib/x64/SDL2.dll)
        message("---- Copy SDL2 DLL")
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2-2.0.16/lib/x64/SDL2.dll DESTINATION .)
    endif()
    #### SDL2 mixer dll copy
    if(EXISTS ${PROJECT_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4/lib/x64/SDL2_mixer.dll)
        message("---- Copy SDL2 Mixer DLL")
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4/lib/x64/SDL2_mixer.dll DESTINATION .)
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4/lib/x64/libFLAC-8.dll DESTINATION .)
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4/lib/x64/libmodplug-1.dll DESTINATION .)
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4/lib/x64/libmpg123-0.dll DESTINATION .)
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4/lib/x64/libogg-0.dll DESTINATION .)
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4/lib/x64/libopus-0.dll DESTINATION .)
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4/lib/x64/libopusfile-0.dll DESTINATION .)
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4/lib/x64/libvorbis-0.dll DESTINATION .)
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4/lib/x64/libvorbisfile-3.dll DESTINATION .)
    endif()
    #### SDL2 ttf dll copy
    if(EXISTS ${PROJECT_SOURCE_DIR}/Externals/SDL2_ttf-2.0.15/lib/x64/SDL2_ttf.dll)
        message("---- Copy SDL2 TTF DLL")
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_ttf-2.0.15/lib/x64/SDL2_ttf.dll DESTINATION .)
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_ttf-2.0.15/lib/x64/zlib1.dll DESTINATION .)
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2_ttf-2.0.15/lib/x64/libfreetype-6.dll DESTINATION .)
    endif()
elseif(UNIX)
    #### Nothing copied
endif()
