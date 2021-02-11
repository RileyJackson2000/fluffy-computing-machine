add_library(
  controller
  controller.cpp)

target_include_directories(controller PUBLIC ../../include)

target_compile_features(controller PUBLIC cxx_std_17)
