#include <iostream>
#include <memory>
#include <random>

#include <chrono>
#include <thread>

#include <utils/constants.hpp>

#include <physics/physics.hpp>
#include <render/render.hpp>
#include <utils/glew.hpp>
#include <utils/glfw.hpp>

#include <model/scene.hpp>
#include <model/object.hpp>

float rfloat()
{
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
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
    s->mat = fcm::STEEL;
    s->position = {(i % 5) * 2.5 - 5,(i / 5) * 2.5 - 5, 0};
    s->velocity = {std::rand()%13-6, std::rand()%13-6, 0};
    s->angular_position = glm::vec3{rfloat(), rfloat(), rfloat()};
    s->angular_velocity = glm::vec3{rfloat(), rfloat(), rfloat()};
    s->name = "sphere";

    scene.insert(std::move(s), glm::vec3{s->radius});
  }

  // main loop

  int nframes = 0;
  int updateInteravl = 30;
  float total = 0;

  do {
    fcm::update(scene, 0.01f); // TODO time steps

    auto start = std::chrono::high_resolution_clock::now();
    viewer.render(scene);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> d = finish - start;
    total += d.count();

    if (++nframes % updateInteravl == 0)
    {
      std::cout << "avg frametime " << total / nframes << "ms" << std::endl;
      nframes = 0;
      total = 0;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  } while (!viewer.closeWindow());

  std::cout << "Goodbye\n";
  return 0;
}
