file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(
  utils
  ${sources}
)

target_include_directories(utils PUBLIC ../../include)

target_compile_features(utils PUBLIC cxx_std_17)
