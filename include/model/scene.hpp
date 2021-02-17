#pragma once

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
  MeshCache *_meshCache;

  RayCaster rayCaster;

public:
  explicit Scene(std::string name, MeshCache *meshCache)
      : _name{std::move(name)}, _meshCache{meshCache}, rayCaster{meshCache} {}
  // can't copy references
  Scene &operator=(const Scene &) = delete;
  Scene &operator=(Scene &&) = delete;
  virtual ~Scene() {}

  const std::string &name() const { return _name; }

  virtual void insert(std::unique_ptr<Object> obj) {
    _objects.emplace_back(std::move(obj));
  }

  const std::vector<std::unique_ptr<Object>> &objects() const {
    return _objects;
  }

  RayCastResult castRay(glm::vec3 pos, glm::vec3 dir) {
    return rayCaster.castRay(pos, dir, _objects);
  }
};

} // namespace fcm
