file( GLOB scene_SOURCES *.cpp )
add_library( scene ${scene_SOURCES})
target_include_directories(scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(scene world Gaemi-00)
