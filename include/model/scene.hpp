#pragma once

#include <model/light.hpp>
#include <model/object.hpp>
#include <model/rayCaster.hpp>
#include <utils/glm.hpp>
#include <utils/types.hpp>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace fcm {

class Scene {
  // std string for testing purposes
protected:
  std::string _name;
  std::vector<std::unique_ptr<Object>> _objects;
  std::vector<std::unique_ptr<DirLight>> _dirLights;
  std::vector<std::unique_ptr<PointLight>> _pointLights;

  RayCaster rayCaster;

public:
  explicit Scene(std::string name) : _name{std::move(name)} {}

  const std::string &name() const { return _name; }

  size_t insertRigidBody(std::unique_ptr<Object>);

  // insert a light and return the index
  size_t insertDirLight(std::unique_ptr<DirLight> light);
  size_t insertPointLight(std::unique_ptr<PointLight> light);

  // not implemented yet
  void removeRigidBody(size_t idx);
  void removeDirLight(size_t idx);
  void removePointLight(size_t idx);

  RayCastResult castRay(glm::vec3 pos, glm::vec3 dir);

  const auto &objects() const { return _objects; }
  const auto &dirLights() const { return _dirLights; }
  const auto &pointLights() const { return _pointLights; }
};

} // namespace fcm
