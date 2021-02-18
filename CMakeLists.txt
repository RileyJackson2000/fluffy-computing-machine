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
  message(STATUS "Doxygen not found, not building docs")
endif()

set(ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)
# leaving this for later if cmake fixes the bug, rn this does nothing
# instead we manually disable the uninstall target in eigen

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(tests)

add_subdirectory(extern/googletest)
add_subdirectory(extern/glew-cmake)
add_subdirectory(extern/glfw)
add_subdirectory(extern/glm)
add_subdirectory(extern/eigen)

