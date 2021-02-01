add_library(
  utils
  utils.cpp)

target_include_directories(utils PUBLIC ../../include)

target_compile_features(utils PUBLIC cxx_std_17)
