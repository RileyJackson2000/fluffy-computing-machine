#pragma once

#include <utils/config.hpp>

#include <model/light.hpp>
#include <model/mesh.hpp>
#include <model/scene.hpp>

#include <render/render.hpp>

#include <server/renderServer.hpp>

#include <memory>
#include <vector>

namespace fcm {

class Server {
  std::unique_ptr<Scene> _scene;

  // this should probably be moved. Ideally we have a global static collection
  // of all the servers
  // the idea is, if _renderServer is nullptr, then we automatically are able
  // to run the simulation headlessly
  RenderServer *_renderServer;

  void dumpToPNG(int uuid);

public:
  Server(std::string sceneName);

  void bindRenderServer(RenderServer *);

  void insertDirLight(std::unique_ptr<DirLight>);
  void insertPointLight(std::unique_ptr<PointLight>);
  void insertRigidBody(std::unique_ptr<Object>);

  // record the simulation for the following number of time steps
  void record(size_t numSteps = 100);

  // run the simulation for the following number time steps
  void run(size_t numSteps = -1);

  const Scene &scene() const;
};

} // namespace fcm
