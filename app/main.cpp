#include <iostream>
#include <memory>

#include <GL/glew.h>

#include <physics/physics.hpp>
#include <render/render.hpp>
#include <utils/glfw.hpp>

#include <model/scene.hpp>
#include <model/object.hpp>


int main(void) {
  std::cout << "Hello there\n";

  std::cout << "Initializing physics...\n";
  fcm::init_physics();
  std::cout << "Initializing renderer...\n";
  auto window = fcm::init_renderer();

  fcm::Scene scene{"Scene 1"};

  // temporarily do this stuff here
  auto s1 = std::make_unique<fcm::Sphere>();
  s1->name = "s1";
  s1->radius = 1;
  s1->mass = 1;
  s1->mat = fcm::STEEL;
  s1->velocity = {2, 0, 0};

  auto s2 = std::make_unique<fcm::Sphere>();
  s2->name = "s2";
  s2->radius = 2;
  s2->mass = 1;
  s2->mat = fcm::STEEL;
  s2->position = {5, 0, 0};
  s2->velocity = {-2, 0, 0};

  scene.insert(std::move(s1));
  scene.insert(std::move(s2));

  // main loop
  // very temporary, only run the simulation for 1 second
  int i = 0;
  do {
    fcm::update(scene, 0.01); // TODO time steps
    fcm::render(scene, window.get());
    i++;
  } while (glfw::glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS and
      glfw::glfwWindowShouldClose(window.get()) == 0 and i < 100);

  fcm::destroy_renderer();

  std::cout << "Goodbye\n";

  return 0;
}
