#pragma once

#include <utils/glm.hpp>

namespace fcm {

struct Camera {
  float fov;
  float aspect;
  glm::vec3 pos;
  glm::vec3 dir;
  glm::vec3 up;

  Camera(float aspect);

  glm::mat4 projectionMat();

  glm::mat4 viewMat();
};

} // namespace fcm
