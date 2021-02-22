#pragma once

#include <model/light.hpp>
#include <model/mesh.hpp>
#include <model/renderScene.hpp>
#include <model/scene.hpp>

#include <render/render.hpp>
#include <render/renderObject.hpp>

#include <utils/config.hpp>

#include <memory>
#include <vector>

namespace fcm {

class Server {
  std::vector<std::unique_ptr<RenderObject>> _renderObjectCache;
  std::unique_ptr<Scene> _scene;
  std::unique_ptr<RenderScene> _renderScene;
  std::unique_ptr<Viewer> _viewer;

public:
  Server(std::string sceneName);
  std::shared_ptr<MeshData> getOrLoadMesh(const std::string &path);
  RenderObjectKey createRenderObject(std::shared_ptr<MeshData>);
  void insertDirLight(std::unique_ptr<DirLight>);
  void insertPointLight(std::unique_ptr<PointLight>);
  void insertRigidBody(std::unique_ptr<Object>);

  // run the simulation for the following number time steps
  void run(size_t numSteps = -1);

  const Scene &scene() const;
  const RenderScene &renderScene() const;
};

} // namespace fcm
