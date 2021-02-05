#pragma once

#include <model/object.hpp>
#include <utils/glm.hpp>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>

#include <render/GLMesh.hpp>
#include <model/mesh.hpp>


namespace fcm {

class Scene {
  // std string for testing purposes
  std::string _name;
  std::vector<std::unique_ptr<Object>> _objects;

public:
  Scene(std::string name): _name{std::move(name)} {}
  const std::string &name() const {return _name;}

  void insert(std::unique_ptr<Object> obj, glm::vec3 scale) { _objects.emplace_back(std::move(obj)); }

  const std::vector<std::unique_ptr<Object>> &objects() const { return _objects; }
};

}
