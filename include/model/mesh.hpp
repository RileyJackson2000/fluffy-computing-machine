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
  glm::vec2 uv{0, 0}; // uv mapping for texture
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
};

inline std::vector<std::unique_ptr<Mesh>> meshCache;

// modify the given mesh to use face normals
void genFaceNormals(Mesh &);

/* generate a mesh, store it in meshCache, and return a pointer to it */
Mesh *genCubeMesh(float sideLength, bool faceNormals = true,
                  bool textures = true);
Mesh *genSphereMesh(float radius, uint32_t sectorCount, uint32_t stackCount,
                    bool faceNormals = true);
Mesh *loadMesh(const std::string &path);

} // namespace fcm
