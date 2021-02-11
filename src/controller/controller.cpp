#include <controller/controller.hpp>
#include <physics/physics.hpp>
#include <utils/constants.hpp>

namespace fcm {

SceneController::SceneController(std::string sceneName, Config config)
    : _config{config}, _rayCaster{&_meshCache}, _camera{float(WINDOW_WIDTH) /
                                                        float(WINDOW_HEIGHT)},
      _scene{std::make_unique<Scene>(sceneName, &_meshCache)},
      _viewer{std::make_unique<Viewer>(&_modelCache, &_rayCaster, &_camera)} {}

MeshKey SceneController::getOrLoadMesh(const std::string &path) {
  (void)path;
  throw "not implemented";
}

MeshKey SceneController::insertMesh(std::unique_ptr<MeshData> mesh) {
  _meshCache.emplace_back(std::move(mesh));
  return _meshCache.size() - 1;
}

ModelKey SceneController::createModel(MeshKey meshKey) {
  _modelCache.emplace_back(std::make_unique<Model>(_meshCache[meshKey].get()));
  return _modelCache.size() - 1;
}

void SceneController::insertRigidBody(std::unique_ptr<Object> obj) {
  _scene->insert(std::move(obj));
}

void SceneController::run(size_t numSteps) {
  // main loop
  double dt = 1.0 / _config.maxTPS;
  double nextTickTarget = _viewer->getTime() + dt;

  int nFrames = 0, nTicks = 0;
  int ticksTillUpdate = _config.maxTPS * 2; // print every 2 seconds
  double totalUpdateTime = 0;

  double timeUpdating = 0, timeRendering = 0;

  for (size_t step = 0; step < numSteps && !_viewer->closeWindow(); ++step) {
    double frameStart = _viewer->getTime();
    fcm::update(_scene.get(), dt);

    timeUpdating += _viewer->getTime() - frameStart;
    double now = _viewer->getTime();
    while (now < nextTickTarget) {
      _viewer->render(_scene.get());
      ++nFrames;
      timeRendering += _viewer->getTime() - now;
      now = _viewer->getTime();
    }
    float frameEnd = _viewer->getTime();

    nextTickTarget += dt;

    ++nTicks;
    totalUpdateTime += frameEnd - frameStart;

    if (nTicks % ticksTillUpdate == 0) {
      std::cout << "Avg TPS: " << nTicks / totalUpdateTime << " | ";
      std::cout << "Avg FPS: " << nFrames / totalUpdateTime << " || ";
      std::cout << " Time spent updating: " << timeUpdating;
      std::cout << " rendering: " << timeRendering;
      std::cout << " iding: " << totalUpdateTime - timeUpdating - timeRendering;
      std::cout << std::endl;
      timeUpdating = timeRendering = 0;
      nFrames = 0;
      nTicks = 0;
      totalUpdateTime = 0;
    }
  }
}

// getter/setters below

const MeshData &SceneController::meshByKey(MeshKey key) {
  return *_meshCache[key];
}

Scene &SceneController::scene() { return *_scene; }

const Config &SceneController::config() const { return _config; }

} // namespace fcm
