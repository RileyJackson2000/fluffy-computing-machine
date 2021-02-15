#pragma once

#include <model/mesh.hpp>
#include <model/scene.hpp>
#include <model/renderScene.hpp>

#include <render/render.hpp>
#include <render/renderObject.hpp>

#include <config.hpp>

#include <memory>
#include <vector>

namespace fcm {

class Server {
  Config _config;
  std::vector<std::unique_ptr<MeshData>> _meshCache;
  std::vector<std::unique_ptr<RenderObject>> _renderObjectCache;
  std::unique_ptr<Scene> _scene;
  std::unique_ptr<RenderScene> _renderScene;
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
  const Scene &scene() const;
  const RenderScene &renderScene() const;
  const Config &config() const;
};

} // namespace fcm
