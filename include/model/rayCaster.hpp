#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include <utils/glm.hpp>

#include <model/object.hpp>

namespace fcm {

// hit result only valid when result == true
struct RayCastResult {
  bool hit;
  float dist;
  glm::vec3 hitLoc;
  glm::vec3 hitNorm;
  size_t triIndex;
  Object *object;

  float timeTaken;
};

struct RayCaster {
  RayCastResult castRay(glm::vec3 pos, glm::vec3 dir,
                        const std::vector<std::unique_ptr<Object>> &objects);
  RayCastResult castRay(glm::vec3 pos, glm::vec3 dir,
                        const std::unique_ptr<Object> &object);

private:
  bool rayTriangleIntersection(glm::vec3 pos, glm::vec3 dir, glm::vec3 v0,
                               glm::vec3 v1, glm::vec3 v2, float &dist,
                               glm::vec3 &hitLoc);
};

} // namespace fcm
