#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

#include <utils/constants.hpp>
#include <utils/glm.hpp>
#include <utils/types.hpp>

#include <model/material.hpp>
#include <model/mesh.hpp>

namespace fcm {

class Object {
protected:
  Object(MeshKey meshKey);
  Object(std::string name, MeshKey meshKey)
      : name{std::move(name)}, meshKey{meshKey} {}

  Object( // only statics
      std::string name, MeshKey meshKey, glm::vec3 position,
      glm::quat orientation, glm::vec3 centroid, Material mat, float mass,
      float moment_of_inertia)
      : name{std::move(name)}, meshKey{meshKey}, position{std::move(position)},
        orientation{std::move(orientation)}, centroid{std::move(centroid)},
        mat{mat}, mass{mass}, moment_of_inertia{moment_of_inertia} {}

  Object( // statics + kinematics
      std::string name, MeshKey meshKey, glm::vec3 position,
      glm::quat orientation, glm::vec3 centroid, glm::vec3 velocity,
      glm::vec3 spin, glm::vec3 force, glm::vec3 torque, Material mat,
      float mass, float moment_of_inertia)
      : name{std::move(name)}, meshKey{meshKey}, position{std::move(position)},
        orientation{std::move(orientation)}, centroid{std::move(centroid)},
        velocity{std::move(velocity)}, spin{std::move(spin)},
        force{std::move(force)}, torque{std::move(torque)}, mat{mat},
        mass{mass}, moment_of_inertia{moment_of_inertia} {}

public:
  virtual ~Object() {}

  std::string name;

  MeshKey meshKey;
  RenderObjectKey renderObjectKey;
  glm::vec3 scale = {1, 1, 1}; // amount to scalein each dir

  glm::vec3 position = {0, 0, 0};
  glm::quat orientation = {1, 0, 0, 0};
  glm::vec3 centroid = {0, 0, 0};

  glm::vec3 velocity = {0, 0, 0};
  glm::vec3 spin = {0, 0, 0}; // angular velocity

  glm::vec3 force = {0, 0, 0};
  glm::vec3 torque = {0, 0, 0};

  Material mat;

  float mass = 0;
  float moment_of_inertia = 0;

  glm::vec3 ambientColour{0.1, 0.f, 0.1f};
  glm::vec3 diffuseColour{0.5f, 0.0f, 0.5f};
  glm::vec3 specColour{1.0, 1.0, 1.0};
  float shininess = 16.0;

  glm::mat4 getTransform() const;
};

struct Sphere : public Object {
  float radius;

  Sphere(std::string name, MeshKey meshKey, float radius, glm::vec3 position,
         Material mat)
      : Object{std::move(name),
               meshKey,
               position,
               {1, 0, 0, 0},
               position,
               mat,
               density(mat) * 4.f / 3.f * PI * radius * radius * radius, // mass
               density(mat) * 8.f / 15.f * PI * radius * radius * radius *
                   radius * radius}, // moment of inertia
        radius{radius} {
    scale = {radius, radius, radius};
  }
};

} // namespace fcm
