#pragma once

#include <utils/glm.hpp>

namespace fcm {

struct Light {
  glm::vec3 ambientColour{0, 0, 0};
  glm::vec3 diffuseColour{0, 0, 0};
  glm::vec3 specularColour{0, 0, 0};
};

struct DirLight : public Light {
  glm::vec3 dir{0, 0, 0};
};

struct PointLight : public Light {
  glm::vec3 pos{0, 0, 0};

  // attenuation
  float constant{1};
  float linear{0};
  float quadratic{0};
};

} // namespace fcm
