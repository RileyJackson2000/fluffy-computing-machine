#pragma once

#include <string>

#include <model/material.hpp>
#include <utils/glm.hpp>

namespace fcm {

struct Object {
  std::string name;
  glm::vec3 position;
  glm::vec3 angular_position;
  glm::vec3 centroid;

  glm::vec3 velocity;
  glm::vec3 angular_velocity;

  glm::vec3 force;
  glm::vec3 torque;

  Material mat;
  float mass;
};

struct Sphere : public Object {
  float radius;
};

}
