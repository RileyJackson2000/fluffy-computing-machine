#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <model/object.hpp>
#include <render/GLMesh.hpp>

namespace fcm {

glm::mat4 Object::getTransform() {
  glm::mat4 t{1.f};
  t = glm::translate(t, position);

  glm::mat4 r = glm::eulerAngleYXZ(angular_position.y, angular_position.x,
                                   angular_position.z);

  return t * r;
}

// this is bad. it has a bunch of copies for no reason. TODO fix this
Object::Object(const MeshData &meshData)
    : meshData{meshData}, glMeshData{meshData} {}

Sphere::Sphere(float radius, bool faceNormals)
    : Object{genSphereMesh(radius, 10, 10, faceNormals)}, radius{radius} {}

} // namespace fcm
