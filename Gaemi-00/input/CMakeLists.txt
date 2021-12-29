file( GLOB input_SOURCES *.cpp )
add_library( input ${input_SOURCES})
target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

## Needed to avoid warning LNK4217
target_compile_definitions(input PUBLIC GEXPORT=1)

target_link_libraries(input SDL2 math)
