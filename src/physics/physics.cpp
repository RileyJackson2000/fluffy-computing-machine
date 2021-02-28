#include <iostream>

#include <model/material.hpp>
#include <physics/collision.hpp>
#include <physics/physics.hpp>
#include <utils/config.hpp>
#include <utils/glm.hpp>

namespace fcm {

void init_physics(void) { init_materials(); }

void iterate_linear(Object *obj, float dt) {
#if DEBUG_MODE == 1
  static bool warned = false;
  if (!warned && obj->mass == 0) {
    std::cerr << "[DEBUG] WARNING! An object(s) has 0 mass, which may cause it"
                 "to behave unexpectedly\n";
    warned = true;
  }
#endif // DEBUG_MODE == 1
  obj->velocity += obj->force / obj->mass * dt;
  obj->position += obj->velocity * dt;
}

void iterate_angular(Object *obj, float dt) {
  const glm::vec3 angular_acceleration = obj->torque / obj->moment_of_inertia;
  obj->spin += angular_acceleration * dt;
  const float spin_angle = glm::length(obj->spin) * dt;
  const glm::vec3 spin_axis = glm::normalize(obj->spin);
  if (spin_angle > EPS) {
    obj->orientation = glm::rotate(obj->orientation, spin_angle, spin_axis);
  }
}

void update(Scene *scene, float dt) {
  // to reduce typing
  const auto &objs = scene->objects();

  for (auto &&obj_ptr : objs) {
    obj_ptr->force = {0, 0, 0};
    obj_ptr->torque = {0, 0, 0};
  }

  // collision check
  // no quad tree yet :p
  for (size_t i = 0; i < objs.size(); ++i) {
    for (size_t j = i + 1; j < objs.size(); ++j) {
      collide(*objs[i].get(), *objs[j].get());
    }
  }

  float a = 0;
  for (auto &&obj_ptr : objs) {
    iterate_linear(obj_ptr.get(), dt);
    iterate_angular(obj_ptr.get(), dt);
    a += obj_ptr->spin.z * obj_ptr->moment_of_inertia;
  }
}

} // namespace fcm
