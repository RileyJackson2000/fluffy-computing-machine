file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(
  physics
  ${sources}
)

target_include_directories(physics PUBLIC ../../include)

target_compile_features(physics PUBLIC cxx_std_17)
