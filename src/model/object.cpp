#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <model/object.hpp>
#include <render/GLMesh.hpp>
#include <string>

namespace fcm {

glm::mat4 Object::getTransform() {
  glm::mat4 t{1.f};
  t = glm::translate(t, position);

  glm::mat4 r = glm::toMat4(orientation);

  return t * r;
}

Object::Object(std::shared_ptr<MeshData> meshData, OBJECT_TYPES objectType)
    : objectType{objectType}, glMeshData{meshData.get()}, meshData{std::move(
                                                              meshData)} {}

Object::Object(std::string name, std::shared_ptr<MeshData> meshData,
               OBJECT_TYPES objectType)
    : objectType{objectType}, name{std::move(name)},
      glMeshData{meshData.get()}, meshData{std::move(meshData)} {}

Object::Object( // only statics
    std::string name, std::shared_ptr<MeshData> meshData, glm::vec3 position,
    glm::quat orientation, glm::vec3 centroid, Material mat, float mass,
    float moment_of_inertia, OBJECT_TYPES objectType)
    : objectType{objectType}, name{std::move(name)}, glMeshData{meshData.get()},
      meshData{std::move(meshData)}, position{std::move(position)},
      orientation{std::move(orientation)}, centroid{std::move(centroid)},
      mat{mat}, mass{mass}, moment_of_inertia{moment_of_inertia} {}

Object::Object( // statics + kinematics
    std::string name, std::shared_ptr<MeshData> meshData, glm::vec3 position,
    glm::quat orientation, glm::vec3 centroid, glm::vec3 velocity,
    glm::vec3 spin, glm::vec3 force, glm::vec3 torque, Material mat, float mass,
    float moment_of_inertia, OBJECT_TYPES objectType)
    : objectType{objectType}, name{std::move(name)}, glMeshData{meshData.get()},
      meshData{std::move(meshData)}, position{std::move(position)},
      orientation{std::move(orientation)}, centroid{std::move(centroid)},
      velocity{std::move(velocity)}, spin{std::move(spin)},
      force{std::move(force)}, torque{std::move(torque)}, mat{mat}, mass{mass},
      moment_of_inertia{moment_of_inertia} {}

Sphere::Sphere(float radius, bool faceNormals)
    : Object{genSphereMesh(radius, 10, 10, faceNormals), SPHERE}, radius{
                                                                      radius} {}

Sphere::Sphere(std::string name, std::shared_ptr<MeshData> meshData,
               glm::vec3 position, float radius, Material mat)
    : Object{std::move(name),
             std::move(meshData),
             position,
             {1, 0, 0, 0},
             position,
             mat,
             density(mat) * 4.f / 3.f * PI * radius * radius * radius, // mass
             density(mat) * 8.f / 15.f * PI * radius * radius * radius *
                 radius * radius,
             SPHERE}, // moment of inertia
      radius{radius} {}

Mesh::Mesh(std::shared_ptr<MeshData> meshData) : Object{meshData, MESH} {}

Mesh::Mesh(std::string name, std::shared_ptr<MeshData> meshData)
    : Object{std::move(name), meshData, MESH} {}

} // namespace fcm
