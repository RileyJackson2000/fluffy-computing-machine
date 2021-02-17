file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(
  render
  ${sources}
)

target_include_directories(render PUBLIC ../../include)

target_compile_features(render PUBLIC cxx_std_17)
