#include <iostream>
#include <memory>

#include <GL/glew.h>

#include <physics/physics.hpp>
#include <render/render.hpp>
#include <utils/glfw.hpp>

#include <model/scene.hpp>


int main(void) {
  std::cout << "Hello there\n";

  std::cout << "Initializing physics...\n";
  fcm::init_physics();
  std::cout << "Initializing renderer...\n";
  auto window = fcm::init_renderer();

  fcm::Scene scene{"Scene 1"};

  // main loop
  do {
    fcm::render(scene, window.get());
  } while (glfw::glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS and
      glfw::glfwWindowShouldClose(window.get()) == 0);

  fcm::destroy_renderer();

  std::cout << "Goodbye\n";

  return 0;
}
