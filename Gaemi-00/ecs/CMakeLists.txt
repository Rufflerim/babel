set(CMAKE_CXX_STANDARD 17)

file( GLOB ecs_SOURCES *.cpp )
add_library( ecs ${ecs_SOURCES})
target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ecs)
