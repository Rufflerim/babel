set(SDL2_ttf_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_ttf_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/x64")
else ()
  set(SDL2_ttf_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/x86")
endif ()

set(SDL2_ttf_LIBRARIES "${SDL2_ttf_LIB_PATH}/SDL2_ttf.lib;")

string(STRIP "${SDL2_ttf_LIB_PATH}" SDL2_ttf_LIB_PATH)
string(STRIP "${SDL2_ttf_LIBRARIES}" SDL2_ttf_LIBRARIES)