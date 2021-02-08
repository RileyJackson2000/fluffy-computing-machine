#include <render/globject.hpp>

namespace fcm {
glm::mat4 GLObject::getTransform() const {
  glm::mat4 t{1.f};
  t = glm::translate(t, _object->position);

  glm::mat4 r = glm::toMat4(_object->orientation);

  return t * r;
}
}
