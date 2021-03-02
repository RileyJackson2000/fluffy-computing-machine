#pragma once

#include <tuple>

#include <utils/glm.hpp>

#include <model/mesh.hpp>

namespace fcm {

void transformMeshPoints(Mesh &mesh, glm::mat4 transform);

float computeVolume(const Mesh &mesh);
float computeDensity(const Mesh &mesh, float mass) {
  return mass / computeVolume(mesh);
}
float computeMass(const Mesh &mesh, float density) {
  return density * computeVolume(mesh);
}

std::tuple<float, glm::vec3, glm::mat3> computeMassProps(const Mesh &mesh,
                                                         float density);

} // namespace fcm
