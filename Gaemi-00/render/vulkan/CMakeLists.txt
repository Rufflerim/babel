file( GLOB render_vulkan_SOURCES *.cpp )
add_library( render_vulkan ${render_vulkan_SOURCES} Device.h)
target_include_directories(render_vulkan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

## Needed to avoid warning LNK4217
target_compile_definitions(render_vulkan PUBLIC GEXPORT=1)

target_link_libraries(render_vulkan Vulkan::Vulkan render SDL2 math glm)
