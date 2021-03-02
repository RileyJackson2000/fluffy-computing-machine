#include <physics/physics.hpp>
#include <server/server.hpp>
#include <utils/constants.hpp>
#include <utils/pngwriter.hpp>

namespace fcm {

Server::Server(std::string sceneName)
    : _scene{std::make_unique<Scene>(std::move(sceneName))} {}

void Server::bindRenderServer(RenderServer *renderServer) {
  _renderServer = renderServer;
}

void Server::insertDirLight(std::unique_ptr<DirLight> light) {
  _scene->insertDirLight(std::move(light));
}

void Server::insertPointLight(std::unique_ptr<PointLight> light) {
  _scene->insertPointLight(std::move(light));
}

void Server::insertRigidBody(std::unique_ptr<Object> obj) {
  _scene->insertRigidBody(std::move(obj));
}

void Server::record(size_t numSteps) {
  if (_renderServer == nullptr) {
    std::cerr << "fatal error: no rendering server found\n";
    return;
  }
  double dt = 1.0 / config.maxTPS;
  std::vector<GLfloat> pixels(3 * config.windowWidth * config.windowHeight);

  for (size_t step = 0; step < numSteps; ++step) {
    _renderServer->render(*_scene);
    {
      glReadPixels(0, 0, config.windowWidth, config.windowWidth, GL_RGB,
                   GL_FLOAT, &pixels[0]);
      pngwriter png(config.windowWidth, config.windowHeight, 0,
                    (config.outPath + std::to_string(step) + ".png").c_str());
      for (size_t y = 0; y < config.windowHeight; ++y) {
        for (size_t x = 0; x < config.windowWidth; ++x) {
          GLfloat *rgb(&pixels[0] + 3 * (y * config.windowWidth + x));
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
  double dt = 1.0 / config.maxTPS;
  double nextTickTarget = _renderServer->getTime() + dt;

  const bool renderAvailable = _renderServer != nullptr;

  int nFrames = 0, nTicks = 0;
  int ticksTillUpdate = config.maxTPS * 2; // print every 2 seconds
  double totalUpdateTime = 0;

  double timeUpdating = 0, timeRendering = 0;

  for (size_t step = 0;
       step < numSteps && renderAvailable && !_renderServer->shouldClose();
       ++step) {
    double frameStart = _renderServer->getTime();
    fcm::update(_scene.get(), dt);

    timeUpdating += _renderServer->getTime() - frameStart;
    double now = _renderServer->getTime();
    while (now < nextTickTarget) {
      if (_renderServer != nullptr) {
        _renderServer->render(*_scene);
      }
      ++nFrames;
      timeRendering += _renderServer->getTime() - now;
      now = _renderServer->getTime();
    }
    float frameEnd = _renderServer->getTime();

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

} // namespace fcm
