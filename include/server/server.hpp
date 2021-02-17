#pragma once

#include <model/mesh.hpp>
#include <model/rayCaster.hpp>
#include <model/scene.hpp>

#include <render/render.hpp>
#include <render/renderObject.hpp>

#include <server/config.hpp>

#include <memory>
#include <vector>

namespace fcm {

class Server {
  Config _config;
  RayCaster _rayCaster;
  Camera _camera;
  std::vector<std::unique_ptr<MeshData>> _meshCache;
  std::vector<std::unique_ptr<RenderObject>> _renderObjectCache;
  std::unique_ptr<Scene> _scene;
  std::unique_ptr<Viewer> _viewer;

public:
  Server(std::string sceneName, Config);
  MeshKey getOrLoadMesh(const std::string &path);
  MeshKey insertMesh(std::unique_ptr<MeshData>);
  RenderObjectKey createRenderObject(MeshKey);
  void insertRigidBody(std::unique_ptr<Object>);

  // run the simulation for the following number time steps
  void run(size_t numSteps = -1);

  const MeshData &meshByKey(MeshKey key);
  Scene &scene();
  const Config &config() const;
};

} // namespace fcm
