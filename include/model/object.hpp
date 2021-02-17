#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

#include <utils/constants.hpp>
#include <utils/glm.hpp>

#include <model/material.hpp>
#include <model/mesh.hpp>
#include <render/GLMesh.hpp>

namespace fcm {

enum OBJECT_TYPES { SPHERE = 0, MESH = 1 };

struct Object {
protected:
  Object(std::shared_ptr<MeshData> meshData, OBJECT_TYPES objectType);
  Object(std::string name, std::shared_ptr<MeshData> meshData,
         OBJECT_TYPES objectType);

  Object( // only statics
      std::string name, std::shared_ptr<MeshData> meshData, glm::vec3 position,
      glm::quat orientation, glm::vec3 centroid, Material mat, float mass,
      float moment_of_inertia, OBJECT_TYPES objectType);

  Object( // statics + kinematics
      std::string name, std::shared_ptr<MeshData> meshData, glm::vec3 position,
      glm::quat orientation, glm::vec3 centroid, glm::vec3 velocity,
      glm::vec3 spin, glm::vec3 force, glm::vec3 torque, Material mat,
      float mass, float moment_of_inertia, OBJECT_TYPES objectType);

public:
  virtual ~Object() {}

  OBJECT_TYPES objectType;

  std::string name;

  GLMeshData glMeshData;
  std::shared_ptr<MeshData> meshData;

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

  glm::mat4 getTransform();
};

struct Sphere : public Object {
  float radius;

  Sphere(float radius, bool faceNormals = true);

  Sphere(std::string name, std::shared_ptr<MeshData> meshData,
         glm::vec3 position, float radius, Material mat);
};

struct Mesh : public Object {
  Mesh(std::shared_ptr<MeshData> meshData);

  Mesh(std::string name, std::shared_ptr<MeshData> meshData);
};

} // namespace fcm
