cmake_minimum_required(VERSION 3.20)
project(Babel)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Executable
add_executable(Babel Babel/Main.cpp)
set_property(TARGET Babel PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Babel>")
target_link_libraries(Babel Babel-Game Gaemi-00)

## Debug status and optimisation flag
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC DEBUG=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC DEBUG=0)
endif()

target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
        -Wall -Werror -Werror=uninitialized

        # SDL compatibility
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-reserved-id-macro
        -Wno-c11-extensions

        # Optimization
        $<$<CONFIG:DEBUG>:-Og>
        $<$<CONFIG:RELEASE>:-Os>
)

## Build
add_subdirectory( Externals )
add_subdirectory( Gaemi-00 )
add_subdirectory(Babel)

