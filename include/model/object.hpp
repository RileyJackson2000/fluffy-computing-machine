#pragma once

#include <string>

#include <utils/glm.hpp>

#include <model/material.hpp>
#include <render/GLMesh.hpp>

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

  std::string meshFile; // REMOVE - objects should eventually be created from meshes instead of the other way around
  GLMesh mesh;
};

struct Sphere : public Object {
  float radius;
};

}
