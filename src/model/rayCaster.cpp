#include <chrono>
#include <memory>
#include <vector>

#include <utils/glm.hpp>

#include <model/object.hpp>
#include <model/rayCaster.hpp>
#include <utils/constants.hpp>

namespace fcm {

RayCastResult
RayCaster::castRay(glm::vec3 pos, glm::vec3 dir,
                   const std::vector<std::unique_ptr<Object>> &objects) {
  auto start = std::chrono::high_resolution_clock::now();

  RayCastResult result;
  result.hit = false;
  result.dist = INF;

  for (const auto &obj : objects) {
    RayCastResult objResult = castRay(pos, dir, obj);
    if (objResult.hit && objResult.dist < result.dist) {
      result = objResult;
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  result.timeTaken =
      std::chrono::duration<float, std::milli>(end - start).count();

  return result;
}

RayCastResult RayCaster::castRay(glm::vec3 pos, glm::vec3 dir,
                                 const std::unique_ptr<Object> &object) {
  RayCastResult result;
  result.hit = false;
  result.dist = INF;

  glm::mat4 modelMatrix = object->getTransform();

  const MeshData *mesh = object->meshData.get();
  for (size_t i = 0; i < mesh->indices.size(); i += 3) {
    glm::vec3 v0{modelMatrix *
                 glm::vec4(mesh->vertices[mesh->indices[i + 0]].pos, 1.f)};
    glm::vec3 v1{modelMatrix *
                 glm::vec4(mesh->vertices[mesh->indices[i + 1]].pos, 1.f)};
    glm::vec3 v2{modelMatrix *
                 glm::vec4(mesh->vertices[mesh->indices[i + 2]].pos, 1.f)};

    float dist = INF;
    glm::vec3 hitLoc;
    if (rayTriangleIntersection(pos, dir, v0, v1, v2, dist, hitLoc)) {
      result.hit = true;
      if (dist < result.dist) {
        result.dist = dist;
        result.hitLoc = hitLoc;
        result.hitNorm = glm::cross(v1 - v0, v2 - v0);
        result.triIndex = i / 3;
        result.object = object.get();
      }
    }
  }

  return result;
}

bool RayCaster::rayTriangleIntersection(glm::vec3 pos, glm::vec3 dir,
                                        glm::vec3 v0, glm::vec3 v1,
                                        glm::vec3 v2, float &dist,
                                        glm::vec3 &hitLoc) {
  glm::vec3 edge1 = v1 - v0, edge2 = v2 - v0;
  glm::vec3 h, s, q;
  float a, f, u, v;
  h = glm::cross(dir, edge2);
  a = glm::dot(edge1, h);
  if (a > -eps && a < eps)
    return false; // This ray is parallel to this triangle.
  f = 1.0 / a;
  s = pos - v0;
  u = f * glm::dot(s, h);
  if (u < 0.0 || u > 1.0)
    return false;
  q = glm::cross(s, edge1);
  v = f * glm::dot(dir, q);
  if (v < 0.0 || u + v > 1.0)
    return false;
  // At this stage we can compute t to find out where the intersection point is
  // on the line.
  float t = f * glm::dot(edge2, q);
  if (t > eps && t < 1 / eps) // ray intersection
  {
    dist = t;
    hitLoc = pos + pos * t;
    return true;
  } else // This means that there is a line intersection but not a ray
         // intersection.
    return false;
}

} // namespace fcm
