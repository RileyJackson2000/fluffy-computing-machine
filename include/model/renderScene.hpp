#pragma once

#include <model/camera.hpp>
#include <model/light.hpp>
#include <model/rayCaster.hpp>
#include <model/scene.hpp>

#include <vector>

namespace fcm {

class RenderScene {
  // right now we only have support for one dir light, but in the future
  // we might want multiple
  std::vector<std::unique_ptr<DirLight>> _dirLights;
  std::vector<std::unique_ptr<PointLight>> _pointLights;

public:
  Scene *scene;
  RayCaster rayCaster;
  Camera camera;

  RenderScene(Scene *scene, RayCaster rayCaster, Camera camera)
      : scene{scene}, rayCaster{rayCaster}, camera{camera} {}

  // insert a light and return the index
  size_t insertDirLight(std::unique_ptr<DirLight> light);
  size_t insertPointLight(std::unique_ptr<PointLight> light);

  // not implemented yet
  void removeDirLight(size_t idx);
  void removePointLight(size_t idx);

  const auto &dirLights() const { return _dirLights; }
  const auto &pointLights() const { return _pointLights; }
  const auto &objects() const { return scene->objects(); }
};

} // namespace fcm
