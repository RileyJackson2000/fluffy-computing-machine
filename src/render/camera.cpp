#include <render/camera.hpp>

namespace fcm {

Camera::Camera(float aspect) : aspect{aspect} {
  fov = 60.f;

  pos = glm::vec3{0.0, 0.0, 30.0};
  dir = glm::vec3{0.0, 0.0, -1.0};
  up = glm::vec3{0.0, 1.0, 0.0};
}

glm::mat4 Camera::projectionMat() {
  return glm::perspective(glm::radians(fov), aspect, 0.01f, 100.0f);
}

glm::mat4 Camera::viewMat() { return glm::lookAt(pos, pos + dir, up); }

} // namespace fcm
