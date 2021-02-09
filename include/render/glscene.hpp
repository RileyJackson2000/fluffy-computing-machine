#pragma once

#include <memory>
#include <vector>

#include <render/globject.hpp>
#include <model/scene.hpp>

namespace fcm {

class GLScene: public Scene {
  std::vector<GLObject> _globjects;
public:

  explicit GLScene(std::string name): Scene(name) {}
  void insert(std::unique_ptr<Object> obj) override {
    _globjects.emplace_back(obj.get());
    Scene::insert(std::move(obj));
  }
  const std::vector<GLObject>& globjects() const { return _globjects; }
};

} // namespace fcm
