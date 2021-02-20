#include <physics/physics.hpp>
#include <server/server.hpp>
#include <utils/constants.hpp>
#include <utils/pngwriter.hpp>

namespace fcm {

Server::Server(std::string sceneName, Config config)
    : _config{config}, _scene{std::make_unique<Scene>(sceneName, &_meshCache)},
      _renderScene{std::make_unique<RenderScene>(
          _scene.get(), RayCaster{&_meshCache},
          Camera{float(config.windowWidth) / float(config.windowHeight)})},
      _viewer{std::make_unique<Viewer>(config, &_renderObjectCache)} {}

MeshKey Server::getOrLoadMesh(const std::string &path) {
  (void)path;
  throw "not implemented";
}

MeshKey Server::insertMesh(std::unique_ptr<MeshData> mesh) {
  _meshCache.emplace_back(std::move(mesh));
  return _meshCache.size() - 1;
}

RenderObjectKey Server::createRenderObject(MeshKey meshKey) {
  _renderObjectCache.emplace_back(
      std::make_unique<RenderObject>(_meshCache[meshKey].get()));
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

void Server::record(size_t numSteps) {
  double dt = 1.0 / _config.maxTPS;
  std::vector<glew::GLfloat> pixels(3 * _config.windowWidth *
                                    _config.windowHeight);

  for (size_t step = 0; step < numSteps; ++step) {
    _viewer->render(*_renderScene);
    {
      glew::glReadPixels(0, 0, _config.windowWidth, _config.windowWidth, GL_RGB,
                         GL_FLOAT, &pixels[0]);
      pngwriter png(_config.windowWidth, _config.windowHeight, 0,
                    (_config.outPath + std::to_string(step) + ".png").c_str());
      for (size_t y = 0; y < _config.windowHeight; ++y) {
        for (size_t x = 0; x < _config.windowWidth; ++x) {
          glew::GLfloat *rgb(&pixels[0] + 3 * (y * _config.windowWidth + x));
          png.plot(x, y, rgb[0], rgb[1], rgb[2]);
        }
      }
      png.close();
    }
    fcm::update(_scene.get(), dt);
  }
}

void Server::run(size_t numSteps) {
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

const MeshData &Server::meshByKey(MeshKey key) { return *_meshCache[key]; }

const Scene &Server::scene() const { return *_scene; }

const RenderScene &Server::renderScene() const { return *_renderScene; }

const Config &Server::config() const { return _config; }

} // namespace fcm
