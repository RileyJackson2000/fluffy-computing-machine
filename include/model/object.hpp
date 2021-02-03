#pragma once

#include <type_traits>
#include <string>
#include <memory>

#include <model/material.hpp>
#include <utils/constants.hpp>
#include <utils/glm.hpp>

namespace fcm {

class Object {
protected:
  Object() {}
  Object(std::string name): name{std::move(name)} {}
  Object( // only statics
    std::string name, glm::dvec3 position, glm::dvec3 angular_position,
    glm::dvec3 centroid,
    Material mat, double mass, double moment_of_inertia
  ) : name{std::move(name)}, position{std::move(position)},
    angular_position{std::move(angular_position)}, centroid{std::move(centroid)},
    mat{mat}, mass{mass}, moment_of_inertia{moment_of_inertia}
  {}
  Object( // statics + kinematics
    std::string name, glm::dvec3 position, glm::dvec3 angular_position,
    glm::dvec3 centroid, glm::dvec3 velocity, glm::dvec3 angular_velocity,
    glm::dvec3 force, glm::dvec3 torque,
    Material mat, double mass, double moment_of_inertia
  ) : name{std::move(name)}, position{std::move(position)},
    angular_position{std::move(angular_position)}, centroid{std::move(centroid)},
    velocity{std::move(velocity)}, angular_velocity{std::move(angular_velocity)},
    force{std::move(force)}, torque{std::move(torque)},
    mat{mat}, mass{mass}, moment_of_inertia{moment_of_inertia}
  {}
public:
  virtual ~Object() {}
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
  double moment_of_inertia;
};

class Sphere : public Object {
public:
  Sphere() {}
  Sphere(std::string name, glm::dvec3 position, double radius, Material mat)
    : Object{std::move(name), position, {0, 0, 0}, position, mat,
    density(mat) * 4. / 3. * pi * radius * radius * radius, // mass
    density(mat) * 8. / 15. * pi * radius * radius * radius * radius * radius}, // moment of inertia
    radius{radius}
  {}
  double radius;
};

} // namespace fcm
