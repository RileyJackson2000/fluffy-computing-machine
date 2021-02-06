#pragma once

#include <memory>
#include <string>
#include <type_traits>

#include <utils/glm.hpp>

#include <model/material.hpp>
#include <model/mesh.hpp>
#include <render/GLMesh.hpp>

namespace fcm {

class Object {
protected:
  // this is bad. it has a bunch of copies for no reason. TODO fix this
  Object(const MeshData &meshData);
  Object(std::string name, const MeshData &meshData)
      : name{std::move(name)}, meshData{meshData}, glMeshData{meshData} {}

  Object( // only statics
      std::string name, const MeshData &meshData, glm::vec3 position,
      glm::vec3 angular_position, glm::vec3 centroid, Material mat, float mass,
      float moment_of_inertia)
      : name{std::move(name)}, meshData{meshData},
        glMeshData{meshData}, position{std::move(position)},
        angular_position{std::move(angular_position)}, centroid{std::move(
                                                           centroid)},
        mat{mat}, mass{mass}, moment_of_inertia{moment_of_inertia} {}

  Object( // statics + kinematics
      std::string name, const MeshData &meshData, glm::vec3 position,
      glm::vec3 angular_position, glm::vec3 centroid, glm::vec3 velocity,
      glm::vec3 angular_velocity, glm::vec3 force, glm::vec3 torque,
      Material mat, float mass, float moment_of_inertia)
      : name{std::move(name)}, meshData{meshData},
        glMeshData{meshData}, position{std::move(position)},
        angular_position{std::move(angular_position)},
        centroid{std::move(centroid)}, velocity{std::move(velocity)},
        angular_velocity{std::move(angular_velocity)}, force{std::move(force)},
        torque{std::move(torque)}, mat{mat}, mass{mass},
        moment_of_inertia{moment_of_inertia} {}

public:
  virtual ~Object() {}
  std::string name;
  glm::vec3 position;
  glm::vec3 angular_position;
  glm::vec3 centroid;

  glm::vec3 velocity;
  glm::vec3 angular_velocity;

  glm::vec3 force;
  glm::vec3 torque;

  float moment_of_inertia;

  Material mat;
  float mass;

  MeshData meshData;
  GLMeshData glMeshData;

  glm::mat4 getTransform();
};

struct Sphere : public Object {
  float radius;

  Sphere(float radius, bool faceNormals = true);

  Sphere(std::string name, const MeshData &meshData, glm::vec3 position,
         float radius, Material mat)
      : Object{std::move(name),
               meshData,
               position,
               {0, 0, 0},
               position,
               mat,
               density(mat) * 4. / 3. * M_PI * radius * radius *
                   radius, // mass
               density(mat) * 8. / 15. * M_PI * radius * radius * radius *
                   radius * radius}, // moment of inertia
        radius{radius} {}
};

} // namespace fcm
