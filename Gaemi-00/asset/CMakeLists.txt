file( GLOB asset_SOURCES *.cpp )
add_library( asset ${asset_SOURCES})
target_include_directories(asset PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

## Needed to avoid warning LNK4217
target_compile_definitions(asset PUBLIC GEXPORT=1)

target_link_libraries(asset sdl_stb_image SDL2 math)
