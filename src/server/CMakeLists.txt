file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(
  server
  ${sources}
)

target_include_directories(server PUBLIC ../../include)

target_compile_features(server PUBLIC cxx_std_17)
