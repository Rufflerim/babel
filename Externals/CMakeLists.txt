message("-- Integrating external libraries")

## SBT image
message("---- Linking SBT Image")
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb_image)

## SBT image write
message("---- Linking SBT Image Write")
add_library(stb_image_write INTERFACE)
target_include_directories(stb_image_write INTERFACE stb_image_write)

## SDL2
message("---- Linking SDL2")
add_library(SDL2 INTERFACE)

if (WIN32)
    set(SDL2_DIR "${CMAKE_SOURCE_DIR}/Externals/SDL2-2.0.16")
    set(SDL_PATH "${SDL2_DIR}")
    find_package(SDL2 REQUIRED)
    target_include_directories(SDL2 INTERFACE ${SDL2_DIR}/include)
    target_link_directories(SDL2 INTERFACE ${SDL2_LIB_PATH})
elseif (UNIX)
    find_package(SDL2 REQUIRED)
    target_include_directories(SDL2 INTERFACE ${SDL2_DIR}/include)
    target_link_directories(SDL2 INTERFACE ${SDL2_DIR}/lib)
endif ()

target_link_libraries(SDL2 INTERFACE ${SDL2_LIBRARIES})

## SDL2 Mixer
message("---- Linking SDL2 Mixer")
add_library(SDL2_mixer INTERFACE)

if (WIN32)
    set(SDL2_mixer_DIR "${CMAKE_SOURCE_DIR}/Externals/SDL2_mixer-2.0.4")
    set(SDL_mixer_PATH "${SDL2_mixer_DIR}")
    find_package(SDL2_mixer REQUIRED)
    target_include_directories(SDL2_mixer INTERFACE ${SDL2_mixer_DIR}/include)
    target_link_directories(SDL2_mixer INTERFACE ${SDL2_mixer_LIB_PATH})
elseif (UNIX)
    find_package(SDL2_mixer REQUIRED)
    target_include_directories(SDL2_mixer INTERFACE ${SDL2_mixer_DIR}/include)
    target_link_directories(SDL2_mixer INTERFACE ${SDL2_mixer_DIR}/lib)
endif ()

target_link_libraries(SDL2_mixer INTERFACE ${SDL2_mixer_LIBRARIES})

## SDL2 TTF
message("---- Linking SDL2 TTF")
add_library(SDL2_ttf INTERFACE)

if (WIN32)
    set(SDL2_ttf_DIR "${CMAKE_SOURCE_DIR}/Externals/SDL2_ttf-2.0.15")
    set(SDL2_ttf_PATH "${SDL2_ttf_DIR}")
    find_package(SDL2_ttf REQUIRED)
    target_include_directories(SDL2_ttf INTERFACE ${SDL2_ttf_DIR}/include)
    target_link_directories(SDL2_ttf INTERFACE ${SDL2_ttf_LIB_PATH})
elseif (UNIX)
    find_package(SDL2_ttf REQUIRED)
    target_include_directories(SDL2_ttf INTERFACE ${SDL2_ttf_DIR}/include)
    target_link_directories(SDL2_ttf INTERFACE ${SDL2_ttf_DIR}/lib)
endif ()

target_link_libraries(SDL2_ttf INTERFACE ${SDL2_ttf_LIBRARIES})


## -----------------------------------------------------------------------------------------

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