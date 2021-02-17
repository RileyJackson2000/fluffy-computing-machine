file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(
  model
  ${sources}
)

target_include_directories(model PUBLIC ../../include)

target_compile_features(model PUBLIC cxx_std_17)
