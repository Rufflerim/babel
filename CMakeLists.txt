cmake_minimum_required(VERSION 3.20)
project(DumbKnights)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory( Externals )
add_subdirectory( Gaemi-00 )
add_subdirectory( DumbKnights )

## Executable
add_executable(${CMAKE_PROJECT_NAME} DumbKnights/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} DumbKnights-Game Gaemi-00)

## Copy dll
message("-- Copy DLLs")
set(BUILD_DIR cmake-build-debug-clang)
## Windows
if(WIN32)
    #### Engine dll copy
    if(EXISTS ./Gaemi-00/Gaemi-00.dll)
        message("---- Copy Engine DLL")
        file(COPY ${BUILD_DIR}/Gaemi-00/Gaemi-00.dll DESTINATION .)
    endif()
    #### SDL2 dll copy
    if(EXISTS ${PROJECT_SOURCE_DIR}/Externals/SDL2-2.0.16/lib/x64/SDL2.dll)
        message("---- Copy SDL2 DLL")
        file(COPY ${PROJECT_SOURCE_DIR}/Externals/SDL2-2.0.16/lib/x64/SDL2.dll DESTINATION .)
    endif()
elseif(UNIX)
    if(EXISTS ./Gaemi-00/Gaemi-00.so)
        message("---- Copy Engine SO file")
        file(COPY ${BUILD_DIR}/GEngine/libGaemi-00.so DESTINATION .)
    endif()
endif()
