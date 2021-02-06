#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <model/object.hpp>
#include <render/GLMesh.hpp>

namespace fcm {

glm::mat4 Object::getTransform() {
  glm::mat4 t{1.f};
  t = glm::translate(t, position);

  glm::mat4 r = glm::toMat4(orientation);

  return t * r;
}

Object::Object(std::shared_ptr<MeshData> meshData)
    : glMeshData{meshData.get()}, meshData{std::move(meshData)} {}

Sphere::Sphere(float radius, bool faceNormals)
    : Object{genSphereMesh(radius, 10, 10, faceNormals)}, radius{radius} {}

} // namespace fcm
