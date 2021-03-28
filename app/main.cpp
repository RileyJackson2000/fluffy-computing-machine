#include <server/server.hpp>

#include <model/light.hpp>
#include <model/mesh.hpp>
#include <model/object.hpp>
#include <model/scene.hpp>
#include <physics/physics.hpp>
#include <render/render.hpp>
#include <utils/constants.hpp>
#include <utils/glew.hpp>
#include <utils/glfw.hpp>

#include <utils/config.hpp>

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

  std::cout << "Initializing physics...\n";
  fcm::init_physics();
  std::cout << "Initializing renderer...\n";
  // should move opengl initialization here instead of the constructor of window

  fcm::Server server{"Scene 1"};
  fcm::RenderServer renderServer;
  server.bindRenderServer(&renderServer);

  fcm::Mesh *sphereMesh =
      fcm::genSphereMesh(1, 31, 31, fcm::config.faceNormals);
  auto sphereRenderMesh = renderServer.insertMesh(sphereMesh);

  auto yelloTex = renderServer.insertTexture(fcm::Pixel(0xff7f00ff));
  auto arrowTex =
      renderServer.insertTexture(fcm::Sprite{"../assets/uparrow.png"});

  fcm::Mesh *cubeMesh = fcm::genCubeMesh(1, fcm::config.faceNormals);
  auto cubeRenderMesh = renderServer.insertMesh(cubeMesh);

  auto s = std::make_unique<fcm::Sphere>("sphere1", sphereMesh, 1,
                                         glm::vec3{0, 0, 0}, fcm::STEEL);
  s->renderMeshKey = sphereRenderMesh;
  s->textureKey = yelloTex;
  server.insertRigidBody(std::move(s));
  /*

  s = std::make_unique<fcm::Sphere>("sphere2", sphereMesh, 1,
                                    glm::vec3{1.5, 1.5, 0.5}, fcm::STEEL);
  s->renderMeshKey = sphereRenderMesh;
  // forget to set texture!! this makes a pink texture instead of seg faulting
  // :) s->textureKey = yelloTex;
  server.insertRigidBody(std::move(s));
  */

  auto c = std::make_unique<fcm::RigidMesh>("cube1", cubeMesh);
  c->mass = 1;
  c->position = {0, 4, 0};
  c->scale = {2, 2, 2};
  // c->orientation = glm::angleAxis(3.1415f / 6, glm::vec3{0., 0, 1});
  c->renderMeshKey = cubeRenderMesh;
  c->textureKey = arrowTex;
  server.insertRigidBody(std::move(c));

  c = std::make_unique<fcm::RigidMesh>("cube2", cubeMesh);
  c->mass = 1;
  c->position = {2, 0, 0};
  c->scale = {2, 2, 2};
  // c->orientation = glm::angleAxis(3.1415f / 6, glm::vec3{0., 0, 1});
  c->renderMeshKey = cubeRenderMesh;
  c->textureKey = arrowTex;
  server.insertRigidBody(std::move(c));

  c = std::make_unique<fcm::RigidMesh>("ground", cubeMesh);
  c->mass = 1;
  c->position = {0, -32, 0};
  c->scale = {30, 30, 30};
  c->renderMeshKey = cubeRenderMesh;
  c->textureKey = arrowTex;
  server.insertRigidBody(std::move(c));

  // for (int i = 0; i < 25; ++i) {
  // float rad = 1 + std::rand() % 3 / 3. * 0.2;
  // glm::vec3 pos = {(i % 5) * 2.5 - 5, (i / 5) * 2.5 - 5, 0};
  // auto s = std::make_unique<fcm::Sphere>("sphere", sphereMesh, rad, pos,
  // fcm::STEEL);
  // s->velocity = {std::rand() % 13 - 6, std::rand() % 13 - 6, 0};
  // s->renderMeshKey = sphereRenderMesh;
  // s->mass = 3;

  // server.insertRigidBody(std::move(s));
  //}

  std::unique_ptr<fcm::DirLight> dirLight = std::make_unique<fcm::DirLight>();
  dirLight->dir = {1, -1, 0.5};
  dirLight->pos = {0, 10, 0};

  dirLight->ambientColour = {0.1, 0.1, 0.1};
  dirLight->diffuseColour = {1.0, 1.0, 1.0};
  dirLight->specularColour = {1.0, 1.0, 1.0};

  server.insertDirLight(std::move(dirLight));

  dirLight = std::make_unique<fcm::DirLight>();
  dirLight->dir = {1, -1, -0.25};
  dirLight->pos = {3, 10, 0};

  dirLight->ambientColour = {0.0, 0.0, 0.0};
  dirLight->diffuseColour = {0.5, 0.5, 0.5};
  dirLight->specularColour = {0.8, 0.8, 0.8};

  server.insertDirLight(std::move(dirLight));

  // std::unique_ptr<fcm::PointLight> light =
  // std::make_unique<fcm::PointLight>(); light->pos = {0, 20, 0};

  // light->constant = 0.0;
  // light->linear = 0.00000007;
  // light->quadratic = 0.0000007;

  // light->ambientColour = {0.1, 0.1, 0.1};
  // light->diffuseColour = {1.0, 1.0, 1.0};
  // light->specularColour = {1.0, 1.0, 1.0};

  // server.insertPointLight(std::move(light));

  if (fcm::config.record) {
    server.record(100);
  } else {
    server.run();
  }

  std::cout << "Goodbye\n";
  return 0;
}
