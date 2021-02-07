#pragma once

#include <tuple>

#include <utils/glm.hpp>

#include <model/mesh.hpp>

namespace fcm {

void transformMeshPoints(MeshData &mesh, glm::mat4 transform);

float computeVolume(const MeshData &mesh);
float computeDensity(const MeshData &mesh, float mass) {
  return mass / computeVolume(mesh);
}
float computeMass(const MeshData &mesh, float density) {
  return density * computeVolume(mesh);
}

std::tuple<float, glm::vec3, glm::mat3> computeMassProps(const MeshData &mesh,
                                                         float density);

} // namespace fcm
