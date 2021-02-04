#include <iostream>
#include <physics/physics.hpp>
#include <physics/collision.hpp>
#include <model/material.hpp>
#include <utils/glm.hpp>

namespace fcm{

void init_physics(void)
{
  init_materials();
}

void update(Scene &scene, float dt) {
  // to reduce typing
  const auto &objs = scene.objects();

  for (auto &&obj_ptr : objs) {
    obj_ptr->force = {0, 0, 0};
    obj_ptr->torque = {0, 0, 0};
  }

  // collision check
  // no quad tree yet :p
  glm::vec3 force1, force2, torque1, torque2;
  for (size_t i = 0; i < objs.size(); ++i) {
    for (size_t j  = i + 1; j < objs.size(); ++j) {
      collide(
        *static_cast<Sphere*>(objs[i].get()), *static_cast<Sphere*>(objs[j].get()), // TODO super bad! LOL! gotta replace with CRTP
        force1, force2, torque1, torque2
      );
      objs[i]->force += force1;
      objs[j]->force += force2;
      objs[i]->torque += torque1;
      objs[j]->torque += torque2;
    }
  }

  for (auto &&obj_ptr : objs) {
    obj_ptr->angular_velocity += obj_ptr->torque / obj_ptr->mass * dt;
    obj_ptr->velocity += obj_ptr->force / obj_ptr->mass * dt;

    obj_ptr->angular_position += obj_ptr->angular_velocity * dt;
    obj_ptr->position += obj_ptr->velocity * dt;
  }
}

}
