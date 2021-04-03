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

enum class ObjectType { SPHERE = 0, RIGID_MESH = 1 };

struct Object {
protected:
  Object(ObjectType objectType, Mesh *mesh);

  Object(ObjectType objectType, std::string name, Mesh *mesh);

  Object( // only statics
      ObjectType objectType, std::string name, Mesh *mesh, glm::vec3 position,
      glm::quat orientation, glm::vec3 centroid, Material mat, float mass,
      float moment_of_inertia);

  Object( // statics + kinematics
      ObjectType objectType, std::string name, Mesh *mesh, glm::vec3 position,
      glm::quat orientation, glm::vec3 centroid, glm::vec3 velocity,
      glm::vec3 spin, glm::vec3 force, glm::vec3 torque, Material mat,
      float mass, float moment_of_inertia);

public:
  virtual ~Object() {}

  ObjectType objectType;

  std::string name;

  Mesh *mesh{nullptr};
  RenderMeshKey renderMeshKey{0};
  TextureKey textureKey{0};
  glm::vec3 scale{1, 1, 1}; // amount to scalein each dir

  glm::vec3 position{0, 0, 0};
  glm::quat orientation{1, 0, 0, 0};
  glm::vec3 centroid{0, 0, 0};

  glm::vec3 velocity{0, 0, 0};
  glm::vec3 spin{0, 0, 0}; // angular velocity

  glm::vec3 force{0, 0, 0};
  glm::vec3 torque{0, 0, 0};

  Material mat;

  float mass{0};
  float moment_of_inertia{0};

  glm::vec3 ambientColour{1.0f, 1.0f, 1.0f};
  glm::vec3 diffuseColour{1.0f, 1.0f, 1.0f};
  glm::vec3 specularColour{1.0f, 1.0f, 1.0f};
  float shininess{16.0};

  glm::mat4 getTransform() const;
};

struct Sphere : public Object {
  float radius;

  Sphere(std::string name, Mesh *mesh, float radius, glm::vec3 position,
         Material mat);
};

struct RigidMesh : public Object {
  RigidMesh(Mesh *mesh);

  RigidMesh(std::string name, Mesh *mesh);
};

} // namespace fcm
