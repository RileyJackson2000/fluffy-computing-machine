#include <model/scene.hpp>

namespace fcm {

size_t Scene::insertRigidBody(std::unique_ptr<Object> obj) {
  _objects.emplace_back(std::move(obj));
  return _objects.size() - 1;
}

size_t Scene::insertDirLight(std::unique_ptr<DirLight> light) {
  _dirLights.emplace_back(std::move(light));
  return _dirLights.size() - 1;
}

size_t Scene::insertPointLight(std::unique_ptr<PointLight> light) {
  // TODO need to check that we don't exceed max_lights
  _pointLights.emplace_back(std::move(light));
  return _pointLights.size() - 1;
}

RayCastResult Scene::castRay(glm::vec3 pos, glm::vec3 dir) {
  return rayCaster.castRay(pos, dir, _objects);
}

} // namespace fcm
