#pragma once

#include <utils/glm.hpp>

namespace fcm {

struct Light {
  glm::vec3 ambientColour;
  glm::vec3 diffuseColour;
  glm::vec3 specularColour;

  glm::vec3 pos;

  float nearPlane{0.1f};
  float farPlane{20.f};
};

struct DirLight : public Light {
  glm::vec3 dir;
};

struct PointLight : public Light {
  // attenuation
  // light intensity is attenuated by factor of
  // 1. / (1. + constant + linear * d + quadratic * d * d)
  // where d is the distance
  // (1 in denominator is to prevent division by 0 for zero initialized light)
  float constant;
  float linear;
  float quadratic;
};

} // namespace fcm
