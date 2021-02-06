#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

#include <utils/glew.hpp>
#include <utils/glfw.hpp>

#include <model/object.hpp>
#include <model/scene.hpp>
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

  fcm::Scene scene{"Scene 1"};

  for (int i = 0; i < 25; ++i) {
    float r = 1 + std::rand() % 3 / 3. * 0.2;
    auto s = std::make_unique<fcm::Sphere>(r, true);
    s->name = std::string("s") + std::to_string(i);
    s->mass = std::rand() % 2 + 1;
    s->moment_of_inertia = s->mass * s->radius * s->radius;
    s->mat = fcm::STEEL;
    s->position = {(i % 5) * 2.5 - 5, (i / 5) * 2.5 - 5, 0};
    s->velocity = {std::rand() % 13 - 6, std::rand() % 13 - 6, 0};
    s->angular_position = glm::vec3{rfloat(), rfloat(), rfloat()};
    s->angular_velocity = glm::vec3{rfloat(), rfloat(), rfloat()};

    scene.insert(std::move(s));
  }

  // main loop

  int fps = 60;
  double dt = 1.0 / fps;
  double nextFrameTarget = viewer.getTime();

  int nFrames = 0;
  int framesTillUpdate = 120;
  double totalFrameTime = 0;

  while (!viewer.closeWindow()) {
    double frameStart = viewer.getTime();
    fcm::update(scene, dt); // TODO time steps
    viewer.render(scene);

    double now = viewer.getTime();
    ;
    if (now < nextFrameTarget) {
      int msLeftTilNextFrame = int((nextFrameTarget - now) * 1000);
      std::this_thread::sleep_for(
          std::chrono::milliseconds(msLeftTilNextFrame));
    }
    double frameEnd = viewer.getTime();

    nextFrameTarget = nextFrameTarget + dt;

    ++nFrames;
    totalFrameTime += frameEnd - frameStart;

    if (nFrames % framesTillUpdate == 0) {
      std::cout << "Avg FPS: " << nFrames / totalFrameTime << std::endl;
    }
  }

  std::cout << "Goodbye\n";
  return 0;
}
