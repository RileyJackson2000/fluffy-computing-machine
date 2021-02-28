#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <model/object.hpp>
#include <string>

namespace fcm {

Object::Object(ObjectType objectType, Mesh *mesh)
    : objectType{objectType}, mesh{mesh} {}

Object::Object(ObjectType objectType, std::string name, Mesh *mesh)
    : objectType{objectType}, name{std::move(name)}, mesh{mesh} {}

Object::Object( // only statics
    ObjectType objectType, std::string name, Mesh *mesh, glm::vec3 position,
    glm::quat orientation, glm::vec3 centroid, Material mat, float mass,
    float moment_of_inertia)
    : objectType{objectType}, name{std::move(name)}, mesh{mesh},
      position{std::move(position)}, orientation{std::move(orientation)},
      centroid{std::move(centroid)}, mat{mat}, mass{mass},
      moment_of_inertia{moment_of_inertia} {}

Object::Object( // statics + kinematics
    ObjectType objectType, std::string name, Mesh *mesh, glm::vec3 position,
    glm::quat orientation, glm::vec3 centroid, glm::vec3 velocity,
    glm::vec3 spin, glm::vec3 force, glm::vec3 torque, Material mat, float mass,
    float moment_of_inertia)
    : objectType{objectType}, name{std::move(name)}, mesh{mesh},
      position{std::move(position)}, orientation{std::move(orientation)},
      centroid{std::move(centroid)}, velocity{std::move(velocity)},
      spin{std::move(spin)}, force{std::move(force)}, torque{std::move(torque)},
      mat{mat}, mass{mass}, moment_of_inertia{moment_of_inertia} {}

glm::mat4 Object::getTransform() const {
  glm::mat4 t = glm::translate(position);
  glm::mat4 s = glm::scale(scale);
  glm::mat4 r = glm::toMat4(orientation);

  return t * r * s;
}

Sphere::Sphere(std::string name, Mesh *mesh, float radius, glm::vec3 position,
               Material mat)
    : Object{ObjectType::SPHERE,
             std::move(name),
             mesh,
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

RigidMesh::RigidMesh(Mesh *mesh) : Object{ObjectType::RIGID_MESH, mesh} {}

RigidMesh::RigidMesh(std::string name, Mesh *mesh)
    : Object{ObjectType::RIGID_MESH, std::move(name), mesh} {}

} // namespace fcm
