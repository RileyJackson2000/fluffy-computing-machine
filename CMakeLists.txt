cmake_minimum_required (VERSION 3.16.3)

project (
  FluffyComputingMachine
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -g -O3)

include(CTest)

find_package(Doxygen)
if(Doxygen_FOUND)
  add_subdirectory(docs)
else()
  message(STATUS "Doxygen not foun, not building docs")
endif()

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(tests)

add_subdirectory(extern/googletest)
