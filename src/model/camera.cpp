#include <model/camera.hpp>

namespace fcm {

Camera::Camera(float aspect) : aspect{aspect} {
  fov = 60.f;

  pos = glm::vec3{0.f, 0.f, 30.f};
  dir = glm::vec3{0.f, 0.f, -1.f};
  up = glm::vec3{0.f, 1.f, 0.f};
}

glm::mat4 Camera::projectionMat() {
  return glm::perspective(glm::radians(fov), aspect, 0.01f, 300.0f);
}

glm::mat4 Camera::viewMat() { return glm::lookAt(pos, pos + dir, up); }

} // namespace fcm
