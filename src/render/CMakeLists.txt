add_library(
  render
  render.cpp
  shader.cpp
  vertexBuffer.cpp
  indexBuffer.cpp
  vertexArray.cpp
  vertexBufferLayout.cpp)

target_include_directories(render PUBLIC ../../include)

target_compile_features(render PUBLIC cxx_std_17)
