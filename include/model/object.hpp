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

  MeshData meshData;
  GLMeshData glMeshData;

  glm::mat4 getTransform()
  {
    glm::mat4 t{1.f};
    t = glm::translate(t, position);

    glm::mat4 r = glm::eulerAngleYXZ(angular_position.y, angular_position.x, angular_position.z);

    return t * r;
  }

  // this is bad. it has a bunch of copies for no reason. TODO fix this
  Object(const MeshData &meshData)
  : meshData{meshData}, glMeshData{meshData} {}
};

struct Sphere : public Object {
  float radius;

  Sphere(float radius, bool faceNormals = true)
  : Object{genSphereMesh(radius, 50, 50, faceNormals)},
    radius{radius}
  {}
};

}
