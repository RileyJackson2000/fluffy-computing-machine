#pragma once

#include <model/scene.hpp>
#include <model/rayCaster.hpp>
#include <model/camera.hpp>
#include <model/light.hpp>

#include <vector>

namespace fcm {

struct RenderScene {
  Scene *scene;
  RayCaster rayCaster;
  Camera camera;
  std::vector<Light> lights;
  
  RenderScene(Scene *scene, RayCaster rayCaster, Camera camera): scene{scene}, rayCaster{rayCaster}, camera{camera} {}

  const auto& objects() const { return scene->objects(); }
};

} // namespace fcm
