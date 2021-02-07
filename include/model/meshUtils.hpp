#pragma once

#include <tuple>

#include <utils/glm.hpp>

#include <model/mesh.hpp>

namespace fcm {

float computeVolume(const MeshData &mesh);
float computeDensity(const MeshData &mesh, float mass) {
  return mass / computeVolume(mesh);
}
float computeMass(const MeshData &mesh, float density) {
  return density * computeVolume(mesh);
}

std::tuple<glm::vec3, glm::mat3, float> computeMassProps(const MeshData &mesh,
                                                         float density);

} // namespace fcm
