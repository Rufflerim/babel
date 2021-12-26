file( GLOB render_sdl_SOURCES *.cpp )
add_library( render_sdl ${render_sdl_SOURCES})
target_include_directories(render_sdl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(render_sdl SDL2 math)
