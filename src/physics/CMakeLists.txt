add_library(
  physics
  collision.cpp
  physics.cpp)

target_include_directories(physics PUBLIC ../../include)

target_compile_features(physics PUBLIC cxx_std_17)
