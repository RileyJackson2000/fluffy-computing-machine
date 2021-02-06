#include <iostream>
#include <model/material.hpp>
#include <physics/collision.hpp>
#include <physics/physics.hpp>
#include <utils/glm.hpp>

namespace fcm {

void init_physics(void) { init_materials(); }

template <typename T> void _collide_with(T *obj1, Object *obj2) {
  const std::type_info &type = typeid(*obj2);
  if (type == typeid(Sphere)) {
    collide(*obj1, *static_cast<Sphere *>(obj2));
  }
}

void _collide(Object *obj1, Object *obj2) {
  const std::type_info &type = typeid(*obj1);
  if (type == typeid(Sphere)) {
    _collide_with(static_cast<Sphere *>(obj1), obj2);
  }
}

void iterate_linear(Object *obj, float dt) {
  obj->velocity += obj->force / obj->mass * dt;
  obj->position += obj->velocity * dt;
}

void iterate_angular(Object *obj, float dt) {
  obj->angular_velocity += obj->torque / obj->moment_of_inertia * dt;
  obj->angular_position += obj->angular_velocity * dt;
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
  for (size_t i = 0; i < objs.size(); ++i) {
    for (size_t j = i + 1; j < objs.size(); ++j) {
      _collide(objs[i].get(), objs[j].get());
    }
  }

  for (auto &&obj_ptr : objs) {
    iterate_linear(obj_ptr.get(), dt);
    iterate_angular(obj_ptr.get(), dt);
  }
}

} // namespace fcm
