cmake_minimum_required (VERSION 3.16.3)

project (
  FluffyComputingMachine
  VERSION 1.0
  LANGUAGES CXX)

add_compile_options(-Wall -Wextra -g -O3 -flto -mtune=native -march=native -I/usr/include/freetype2 -I/usr/include/libpng16)

set(WORKFLOW FALSE CACHE BOOL "No compiling with user code warnings")

include(CTest)

set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(tests)

add_subdirectory(extern)
