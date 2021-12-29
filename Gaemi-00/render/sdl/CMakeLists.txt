file( GLOB render_sdl_SOURCES *.cpp )
add_library( render_sdl ${render_sdl_SOURCES})
target_include_directories(render_sdl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

## Needed to avoid warning LNK4217
target_compile_definitions(render_sdl PUBLIC GEXPORT=1)

target_link_libraries(render_sdl SDL2 math stb_image)
