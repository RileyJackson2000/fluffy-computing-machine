#pragma once

#include <memory>
#include <vector>

#include <utils/glm.hpp>

namespace fcm {

struct Mesh;

using MeshCache = std::vector<std::unique_ptr<Mesh>>;

struct Vertex {
  glm::vec3 pos;
  glm::vec3 norm;
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  void faceNormals();
};

inline std::vector<std::unique_ptr<Mesh>> meshCache;

/* generate a mesh, store it in meshCache, and return a pointer to it */
Mesh *genCubeMesh(float side, bool faceNormals = true);
Mesh *genSphereMesh(float radius, uint32_t sectorCount, uint32_t stackCount,
                    bool faceNormals = true);
Mesh *loadMesh(const std::string &path);

} // namespace fcm
