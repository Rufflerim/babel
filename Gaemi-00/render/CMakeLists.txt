file( GLOB render_SOURCES *.cpp )
add_library( render ${render_SOURCES})
target_include_directories(render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

## Needed to avoid warning LNK4217
target_compile_definitions(render PUBLIC GEXPORT=1)

target_link_libraries(render SDL2)
