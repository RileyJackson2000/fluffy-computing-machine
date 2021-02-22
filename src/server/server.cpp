#include <memory>
#include <physics/physics.hpp>
#include <server/server.hpp>
#include <utils/config.hpp>
#include <utils/constants.hpp>

namespace fcm {

Server::Server(std::string sceneName)
    : _scene{std::make_unique<Scene>(sceneName)},
      _renderScene{std::make_unique<RenderScene>(
          _scene.get(), RayCaster{},
          Camera{float(Config::windowWidth) / float(Config::windowHeight)})},
      _viewer{std::make_unique<Viewer>(&_renderObjectCache)} {}

std::shared_ptr<MeshData> Server::getOrLoadMesh(const std::string &path) {
  (void)path;
  throw "not implemented";
}

RenderObjectKey Server::createRenderObject(std::shared_ptr<MeshData> mesh) {
  _renderObjectCache.emplace_back(std::make_unique<RenderObject>(mesh));
  return _renderObjectCache.size() - 1;
}

void Server::insertDirLight(std::unique_ptr<DirLight> light) {
  _renderScene->insertDirLight(std::move(light));
}

void Server::insertPointLight(std::unique_ptr<PointLight> light) {
  _renderScene->insertPointLight(std::move(light));
}

void Server::insertRigidBody(std::unique_ptr<Object> obj) {
  _scene->insert(std::move(obj));
}

void Server::run(size_t numSteps) {
  // main loop
  double dt = 1.0 / Config::maxTPS;
  double nextTickTarget = _viewer->getTime() + dt;

  int nFrames = 0, nTicks = 0;
  int ticksTillUpdate = Config::maxTPS * 2; // print every 2 seconds
  double totalUpdateTime = 0;

  double timeUpdating = 0, timeRendering = 0;

  for (size_t step = 0; step < numSteps && !_viewer->closeWindow(); ++step) {
    double frameStart = _viewer->getTime();
    fcm::update(_scene.get(), dt);

    timeUpdating += _viewer->getTime() - frameStart;
    double now = _viewer->getTime();
    while (now < nextTickTarget) {
      _viewer->render(*_renderScene);
      ++nFrames;
      timeRendering += _viewer->getTime() - now;
      now = _viewer->getTime();
    }
    float frameEnd = _viewer->getTime();

    nextTickTarget += dt;

    ++nTicks;
    totalUpdateTime += frameEnd - frameStart;

    if (nTicks % ticksTillUpdate == 0) {
      printf("Avg TPS: %0.2lf | Avg FPS: %0.2lf || ", nTicks / totalUpdateTime,
             nFrames / totalUpdateTime);
      printf("Time spent updating: %0.5lf ", timeUpdating);
      printf("rendering: %0.5lf ", timeRendering);
      printf("idling: %0.5lf\n",
             totalUpdateTime - timeUpdating - timeRendering);
      timeUpdating = timeRendering = 0;
      nFrames = 0;
      nTicks = 0;
      totalUpdateTime = 0;
    }
  }
}

// getter/setters below

const Scene &Server::scene() const { return *_scene; }

const RenderScene &Server::renderScene() const { return *_renderScene; }

} // namespace fcm
