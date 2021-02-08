#pragma once

#include <model/object.hpp>
#include <utils/glm.hpp>

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

public:
  explicit Scene(std::string name) : _name{std::move(name)} {}
  virtual ~Scene() {}

  const std::string &name() const { return _name; }

  virtual void insert(std::unique_ptr<Object> obj) {
    _objects.emplace_back(std::move(obj));
  }

  const std::vector<std::unique_ptr<Object>> &objects() const {
    return _objects;
  }
};

} // namespace fcm
