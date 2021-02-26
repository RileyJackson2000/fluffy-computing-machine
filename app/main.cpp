#include <server/server.hpp>

#include <model/light.hpp>
#include <model/object.hpp>
#include <model/scene.hpp>
#include <physics/physics.hpp>
#include <render/render.hpp>
#include <utils/constants.hpp>
#include <utils/glew.hpp>
#include <utils/glfw.hpp>

#include <config.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

float rfloat() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

int main() {
  std::cout << "Hello there\n";

  fcm::Config config;

  std::cout << "Initializing physics...\n";
  fcm::init_physics();
  std::cout << "Initializing renderer...\n";
  // should move opengl initialization here instead of the constructor of window

  fcm::Server server{"Scene 1", config};
  auto sphereKey =
      server.insertMesh(fcm::genSphereMesh(1, 10, 10, config.faceNormals));
  auto sphereModel = server.createRenderObject(sphereKey);

  for (int i = 0; i < 25; ++i) {
    float rad = 1 + std::rand() % 3 / 3. * 0.2;
    glm::vec3 pos = {(i % 5) * 2.5 - 5, (i / 5) * 2.5 - 5, 0};
    auto s = std::make_unique<fcm::Sphere>("sphere", sphereKey, rad, pos,
                                           fcm::STEEL);
    s->velocity = {std::rand() % 13 - 6, std::rand() % 13 - 6, 0};
    s->renderObjectKey = sphereModel;
    s->mass = 3;

    server.insertRigidBody(std::move(s));
  }

  std::unique_ptr<fcm::DirLight> dirLight = std::make_unique<fcm::DirLight>();
  dirLight->dir = {0, -1, 0};

  dirLight->ambientColour = {0.1, 0.1, 0.1};
  dirLight->diffuseColour = {0.2, 0.2, 0.2};
  dirLight->specularColour = {1.0, 1.0, 1.0};

  server.insertDirLight(std::move(dirLight));

  std::unique_ptr<fcm::PointLight> light = std::make_unique<fcm::PointLight>();
  light->pos = {0, 0, 0};

  light->constant = 1.0;
  light->linear = 0.007;
  light->quadratic = 0.0017;

  light->ambientColour = {0.1, 0.1, 0.1};
  light->diffuseColour = {1.0, 1.0, 1.0};
  light->specularColour = {1.0, 1.0, 1.0};

  server.insertPointLight(std::move(light));

  if (config.record) {
    server.record(100);
  } else {
    server.run();
  }

  std::cout << "Goodbye\n";
  return 0;
}
