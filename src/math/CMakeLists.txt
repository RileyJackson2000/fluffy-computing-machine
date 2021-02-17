file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(
  math
  ${sources})

target_include_directories(math PUBLIC ../../include)

target_compile_features(math PUBLIC cxx_std_17)
