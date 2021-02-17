add_library(
  server
  server.cpp)

target_include_directories(server PUBLIC ../../include)

target_compile_features(server PUBLIC cxx_std_17)
