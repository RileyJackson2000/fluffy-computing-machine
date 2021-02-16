add_library(
  model
  scene.cpp
  renderScene.cpp
  camera.cpp
  material.cpp
  mesh.cpp
  object.cpp
  meshUtils.cpp
  rayCaster.cpp)

target_include_directories(model PUBLIC ../../include)

target_compile_features(model PUBLIC cxx_std_17)
