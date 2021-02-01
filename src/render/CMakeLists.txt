add_library(
  render
  render.cpp)

target_include_directories(render PUBLIC ../../include)

target_compile_features(render PUBLIC cxx_std_17)
