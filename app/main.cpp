#include <server/config.hpp>
#include <server/server.hpp>

#include <model/object.hpp>
#include <model/scene.hpp>
#include <physics/physics.hpp>
#include <render/render.hpp>
#include <utils/constants.hpp>
#include <utils/glew.hpp>
#include <utils/glfw.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

float rfloat() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

int main(void) {
  std::cout << "Hello there\n";

  fcm::Config config;

  std::cout << "Initializing physics...\n";
  fcm::init_physics();
  std::cout << "Initializing renderer...\n";
  // should move opengl initialization here instead of the constructor of window

  fcm::Server ctrl{"Scene 1", config};
  auto sphereKey =
      ctrl.insertMesh(fcm::genSphereMesh(1, 10, 10, config.faceNormals));
  auto sphereModel = ctrl.createRenderObject(sphereKey);

  for (int i = 0; i < 25; ++i) {
    float rad = 1 + std::rand() % 3 / 3. * 0.2;
    glm::vec3 pos = {(i % 5) * 2.5 - 5, (i / 5) * 2.5 - 5, 0};
    auto s = std::make_unique<fcm::Sphere>("sphere", sphereKey, rad, pos,
                                           fcm::STEEL);
    s->velocity = {std::rand() % 13 - 6, std::rand() % 13 - 6, 0};
    s->renderObjectKey = sphereModel;
    s->mass = 3;

    ctrl.insertRigidBody(std::move(s));
  }

  ctrl.run();

  std::cout << "Goodbye\n";
  return 0;
}
