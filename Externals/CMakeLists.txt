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
