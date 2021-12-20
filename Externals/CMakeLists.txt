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
