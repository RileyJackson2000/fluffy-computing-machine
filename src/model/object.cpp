#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <model/object.hpp>
#include <string>

namespace fcm {

  Object::Object(OBJECT_TYPES objectType, MeshKey meshKey): objectType{objectType}, meshKey{meshKey} {}

Object::Object(OBJECT_TYPES objectType, std::string name, MeshKey meshKey): objectType{objectType}, name{std::move(name)}, meshKey{meshKey} {}

Object::Object( // only statics
      OBJECT_TYPES objectType, std::string name, MeshKey meshKey, glm::vec3 position,
      glm::quat orientation, glm::vec3 centroid, Material mat, float mass,
      float moment_of_inertia)
      : objectType{objectType}, name{std::move(name)}, meshKey{meshKey}, position{std::move(position)},
        orientation{std::move(orientation)}, centroid{std::move(centroid)},
        mat{mat}, mass{mass}, moment_of_inertia{moment_of_inertia} {}

Object::Object( // statics + kinematics
      OBJECT_TYPES objectType, std::string name, MeshKey meshKey, glm::vec3 position,
      glm::quat orientation, glm::vec3 centroid, glm::vec3 velocity,
      glm::vec3 spin, glm::vec3 force, glm::vec3 torque, Material mat,
      float mass, float moment_of_inertia)
      : objectType{objectType}, name{std::move(name)}, meshKey{meshKey}, position{std::move(position)},
        orientation{std::move(orientation)}, centroid{std::move(centroid)},
        velocity{std::move(velocity)}, spin{std::move(spin)},
        force{std::move(force)}, torque{std::move(torque)}, mat{mat},
        mass{mass}, moment_of_inertia{moment_of_inertia} {}

glm::mat4 Object::getTransform() const {
  glm::mat4 t{1.f};
  t = glm::scale(t, scale);
  t = glm::translate(t, position);

  glm::mat4 r = glm::toMat4(orientation);

  return t * r;
}


Sphere::Sphere(std::string name, MeshKey meshKey, float radius, glm::vec3 position,
    Material mat)
  : Object{SPHERE, std::move(name),
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


Mesh::Mesh(MeshKey meshKey) : Object{MESH, meshKey} {}

Mesh::Mesh(std::string name, MeshKey meshKey)
  : Object{MESH, std::move(name), meshKey} {}

  } // namespace fcm
