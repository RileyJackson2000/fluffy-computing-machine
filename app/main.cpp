#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

#include <utils/glew.hpp>
#include <utils/glfw.hpp>

#include <model/object.hpp>
#include <model/scene.hpp>
#include <render/glscene.hpp>
#include <physics/physics.hpp>
#include <render/render.hpp>
#include <utils/constants.hpp>

float rfloat() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

int main(void) {
  std::cout << "Hello there\n";

  std::cout << "Initializing physics...\n";
  fcm::init_physics();
  std::cout << "Initializing renderer...\n";
  fcm::Viewer viewer{};

  std::unique_ptr<fcm::Scene> scene = std::make_unique<fcm::GLScene>("Scene 1");

  for (int i = 0; i < 25; ++i) {
    float r = 1 + std::rand() % 3 / 3. * 0.2;
    auto s = std::make_unique<fcm::Sphere>(r, true);
    s->name = std::string("s") + std::to_string(i);
    s->mass = std::rand() % 2 + 1;
    s->moment_of_inertia = s->mass * s->radius * s->radius;
    s->mat = fcm::STEEL;
    s->position = {(i % 5) * 2.5 - 5, (i / 5) * 2.5 - 5, 0};
    s->velocity = {std::rand() % 13 - 6, std::rand() % 13 - 6, 0};

    scene->insert(std::move(s));
  }

  // main loop

  int tps = 60; // tick per second
  double dt = 1.0 / tps;
  double nextTickTarget = viewer.getTime() + dt;

  int nFrames = 0, nTicks = 0;
  int ticksTillUpdate = tps * 2; // print every 2 seconds
  double totalUpdateTime = 0;

  double timeUpdating = 0, timeRendering = 0;

  while (!viewer.closeWindow()) {
    double frameStart = viewer.getTime();
    fcm::update(scene.get(), dt);

    timeUpdating += viewer.getTime() - frameStart;
    double now = viewer.getTime();
    while (now < nextTickTarget) {
      viewer.render(static_cast<fcm::GLScene*>(scene.get()));
      ++nFrames;
      timeRendering += viewer.getTime() - now;
      now = viewer.getTime();
    }
    float frameEnd = viewer.getTime();

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

  std::cout << "Goodbye\n";
  return 0;
}
