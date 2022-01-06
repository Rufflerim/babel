file( GLOB comp_SOURCES *.cpp )
add_library( comp ${comp_SOURCES})
target_include_directories(comp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(comp Gaemi-00 SDL2)
