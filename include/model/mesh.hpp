#pragma once

#include <memory>
#include <vector>

#include <utils/glm.hpp>

namespace fcm {

struct Vertex {
  glm::vec3 pos;
  glm::vec3 norm;
};

struct MeshData {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  void faceNormals();
};

std::unique_ptr<MeshData> genCubeMesh(float side, bool faceNormals = true);
std::unique_ptr<MeshData> genSphereMesh(float radius, uint32_t sectorCount,
                                        uint32_t stackCount,
                                        bool faceNormals = true);

} // namespace fcm
