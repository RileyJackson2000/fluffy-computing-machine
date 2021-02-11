add_library(
  math
  lcp.cpp
  lp.cpp
  qcp.cpp)

target_include_directories(math PUBLIC ../../include)

target_compile_features(math PUBLIC cxx_std_17)
