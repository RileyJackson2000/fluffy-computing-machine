#pragma once

#include <memory>
#include <string>
#include <type_traits>

#include <utils/constants.hpp>
#include <utils/glm.hpp>

#include <model/material.hpp>
#include <model/mesh.hpp>
#include <render/GLMesh.hpp>

namespace fcm {

class Object {
protected:
  Object(std::shared_ptr<MeshData> meshData);
  Object(std::string name, std::shared_ptr<MeshData> meshData)
      : name{std::move(name)}, glMeshData{meshData.get()}, meshData{std::move(
                                                               meshData)} {}

  Object( // only statics
      std::string name, std::shared_ptr<MeshData> meshData, glm::vec3 position,
      glm::vec3 angular_position, glm::vec3 centroid, Material mat, float mass,
      float moment_of_inertia)
      : name{std::move(name)}, glMeshData{meshData.get()},
        meshData{std::move(meshData)}, position{std::move(position)},
        angular_position{std::move(angular_position)}, centroid{std::move(
                                                           centroid)},
        mat{mat}, mass{mass}, moment_of_inertia{moment_of_inertia} {}

  Object( // statics + kinematics
      std::string name, std::shared_ptr<MeshData> meshData, glm::vec3 position,
      glm::vec3 angular_position, glm::vec3 centroid, glm::vec3 velocity,
      glm::vec3 angular_velocity, glm::vec3 force, glm::vec3 torque,
      Material mat, float mass, float moment_of_inertia)
      : name{std::move(name)}, glMeshData{meshData.get()},
        meshData{std::move(meshData)}, position{std::move(position)},
        angular_position{std::move(angular_position)},
        centroid{std::move(centroid)}, velocity{std::move(velocity)},
        angular_velocity{std::move(angular_velocity)}, force{std::move(force)},
        torque{std::move(torque)}, mat{mat}, mass{mass},
        moment_of_inertia{moment_of_inertia} {}

public:
  virtual ~Object() {}

  std::string name;

  GLMeshData glMeshData;
  std::shared_ptr<MeshData> meshData;

  glm::vec3 position;
  glm::vec3 angular_position;
  glm::vec3 centroid;

  glm::vec3 velocity;
  glm::vec3 angular_velocity;

  glm::vec3 force;
  glm::vec3 torque;

  Material mat;

  float mass;
  float moment_of_inertia;

  glm::mat4 getTransform();
};

struct Sphere : public Object {
  float radius;

  Sphere(float radius, bool faceNormals = true);

  Sphere(std::string name, std::shared_ptr<MeshData> meshData,
         glm::vec3 position, float radius, Material mat)
      : Object{std::move(name),
               std::move(meshData),
               position,
               {0, 0, 0},
               position,
               mat,
               density(mat) * 4.f / 3.f * PI * radius * radius * radius, // mass
               density(mat) * 8.f / 15.f * PI * radius * radius * radius *
                   radius * radius}, // moment of inertia
        radius{radius} {}
};

} // namespace fcm
