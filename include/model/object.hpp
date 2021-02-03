#pragma once

#include <string>

#include <model/material.hpp>
#include <utils/glm.hpp>

namespace fcm {

struct Object {
  std::string name;
  glm::dvec3 position;
  glm::dvec3 angular_position;
  glm::dvec3 centroid;

  glm::dvec3 velocity;
  glm::dvec3 angular_velocity;

  glm::dvec3 force;
  glm::dvec3 torque;

  Material mat;
  double mass;
};

struct Sphere : public Object {
  double radius;
};

}
