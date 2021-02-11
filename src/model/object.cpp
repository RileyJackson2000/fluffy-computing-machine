#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <model/object.hpp>

namespace fcm {

Object::Object(MeshKey meshKey) : meshKey{meshKey} {}

// Sphere::Sphere(float radius, bool faceNormals)
//: Object{genSphereMesh(radius, 10, 10, faceNormals)}, radius{radius} {}

glm::mat4 Object::getTransform() const {
  glm::mat4 t{1.f};
  t = glm::scale(t, scale);
  t = glm::translate(t, position);

  glm::mat4 r = glm::toMat4(orientation);

  return t * r;
}

} // namespace fcm
