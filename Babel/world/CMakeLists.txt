file( GLOB world_SOURCES *.cpp )
add_library( world ${world_SOURCES})
target_include_directories(world PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(world Gaemi-00)
