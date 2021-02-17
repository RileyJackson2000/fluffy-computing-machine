#pragma once

#include <utils/glm.hpp>

namespace fcm {

struct Light {
  glm::vec3 ambientColour;
  glm::vec3 diffuseColour;
  glm::vec3 specularColour;
};

struct DirLight : public Light {
  glm::vec3 dir;
};

struct PointLight : public Light {
  glm::vec3 pos;

  // attenuation
  float constant;
  float linear;
  float quadratic;
};

} // namespace fcm
