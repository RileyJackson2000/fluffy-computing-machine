#pragma once

#include <model/mesh.hpp>
#include <model/rayCaster.hpp>
#include <model/scene.hpp>

#include <render/model.hpp>
#include <render/render.hpp>

#include <controller/config.hpp>

#include <memory>
#include <vector>

namespace fcm {

class SceneController {
  Config _config;
  RayCaster _rayCaster;
  Camera _camera;
  std::vector<std::unique_ptr<MeshData>> _meshCache;
  std::vector<std::unique_ptr<Model>> _modelCache;
  std::unique_ptr<Scene> _scene;
  std::unique_ptr<Viewer> _viewer;

public:
  SceneController(std::string sceneName, Config);
  MeshKey getOrLoadMesh(const std::string &path);
  MeshKey insertMesh(std::unique_ptr<MeshData>);
  ModelKey createModel(MeshKey);
  void insertRigidBody(std::unique_ptr<Object>);

  // run the simulation for the following number time steps
  void run(size_t numSteps = -1);

  const MeshData &meshByKey(MeshKey key);
  Scene &scene();
  const Config &config() const;
};

} // namespace fcm
