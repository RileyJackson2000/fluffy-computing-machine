#include <model/renderScene.hpp>

namespace fcm {

size_t RenderScene::insertDirLight(std::unique_ptr<DirLight> light) {
  _dirLights.emplace_back(std::move(light));
  return _dirLights.size() - 1;
}

size_t RenderScene::insertPointLight(std::unique_ptr<PointLight> light) {
  // TODO need to check that we don't exceed max_lights
  _pointLights.emplace_back(std::move(light));
  return _pointLights.size() - 1;
}

} // namespace fcm
