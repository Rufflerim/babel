file( GLOB data_SOURCES *.cpp )
add_library( data ${data_SOURCES})
target_include_directories(data PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(data Gaemi-00)
