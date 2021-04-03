#include <iostream>

#include <model/mesh.hpp>
#include <utils/constants.hpp>

namespace fcm {

void genFaceNormals(Mesh &mesh) {
  std::vector<Vertex> tempVerts;
  tempVerts.reserve(mesh.indices.size());

  std::vector<uint32_t> tempInds;
  tempInds.reserve(mesh.indices.size());

  for (uint32_t i = 0; i < mesh.indices.size(); i += 3) {
    Vertex v0 = mesh.vertices[mesh.indices[i + 0]];
    Vertex v1 = mesh.vertices[mesh.indices[i + 1]];
    Vertex v2 = mesh.vertices[mesh.indices[i + 2]];
    glm::vec3 a = v0.pos;
    glm::vec3 b = v1.pos;
    glm::vec3 c = v2.pos;
    glm::vec3 surfaceNorm = glm::normalize(glm::cross(b - a, c - a));

    Vertex a_new = {a, surfaceNorm, v0.uv};
    Vertex b_new = {b, surfaceNorm, v1.uv};
    Vertex c_new = {c, surfaceNorm, v2.uv};

    tempVerts.push_back(a_new);
    tempVerts.push_back(b_new);
    tempVerts.push_back(c_new);

    tempInds.push_back(i + 0);
    tempInds.push_back(i + 1);
    tempInds.push_back(i + 2);
  }

  mesh.vertices = tempVerts;
  mesh.indices = tempInds;
}

Mesh *genCubeMesh(float sideLength, bool faceNormals, bool textures) {
  auto md = std::make_unique<Mesh>();
  float verts[] = {
      // front
      -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
      // back
      -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0};

  uint32_t tris[] = {// front
                     0, 1, 2, 2, 3, 0,
                     // right
                     1, 5, 6, 6, 2, 1,
                     // back
                     7, 6, 5, 5, 4, 7,
                     // left
                     4, 0, 3, 3, 7, 4,
                     // bottom
                     4, 5, 1, 1, 0, 4,
                     // top
                     3, 2, 6, 6, 7, 3};

  for (uint32_t i = 0; i < 8 * 3; i += 3) {
    Vertex v;
    v.pos = sideLength * glm::vec3{verts[i + 0], verts[i + 1], verts[i + 2]};
    v.norm = glm::normalize(v.pos);
    v.uv = {0, 0};
    md->vertices.push_back(v);
  }

  for (uint32_t i = 0; i < 6 * 2 * 3; ++i) {
    md->indices.push_back(tris[i]);
  }

  if (faceNormals) {
    genFaceNormals(*md);
  }

  if (textures) {
    float uvs[] = {// bottom right triangle
                   00.0, 0.0, 1.0, 0.0, 1.0, 1.0,
                   // top left triangle
                   1.0, 1.0, 0.0, 1.0, 0.0, 0.0};
    std::vector<Vertex> tempVerts;
    tempVerts.reserve(md->indices.size());

    std::vector<uint32_t> tempInds;
    tempInds.reserve(md->indices.size());

    int uv_idx = 0;
    for (uint32_t i = 0; i < md->indices.size(); i += 3) {
      Vertex v0 = md->vertices[md->indices[i + 0]];
      Vertex v1 = md->vertices[md->indices[i + 1]];
      Vertex v2 = md->vertices[md->indices[i + 2]];

      v0.uv = {uvs[uv_idx + 0], uvs[uv_idx + 1]};
      v1.uv = {uvs[uv_idx + 2], uvs[uv_idx + 3]};
      v2.uv = {uvs[uv_idx + 4], uvs[uv_idx + 5]};
      uv_idx = 6 - uv_idx; // alternate between 0 and 6

      tempVerts.push_back(v0);
      tempVerts.push_back(v1);
      tempVerts.push_back(v2);

      tempInds.push_back(i + 0);
      tempInds.push_back(i + 1);
      tempInds.push_back(i + 2);
    }
    md->vertices = tempVerts;
    md->indices = tempInds;
  }

  meshCache.emplace_back(std::move(md));

  return meshCache.back().get();
}

Mesh *genSphereMesh(float radius, uint32_t sectorCount, uint32_t stackCount,
                    bool faceNormals) {
  auto md = std::make_unique<Mesh>();

  float x, y, z, xy; // vertex position

  float sectorStep = 2.0 * PI / float(sectorCount);
  float stackStep = PI / float(stackCount);
  float sectorAngle, stackAngle;

  for (uint32_t i = 0; i <= stackCount; ++i) {
    stackAngle = PI / 2.0 - i * stackStep; // starting from pi/2 to -pi/2
    xy = radius * std::cos(stackAngle);    // r * cos(u)
    z = radius * std::sin(stackAngle);     // r * sin(u)

    // add (sectorCount+1) vertices per stack
    // the first and last vertices have same position and normal, but different
    // tex coords
    for (uint32_t j = 0; j <= sectorCount; ++j) {
      Vertex v;
      sectorAngle = j * sectorStep; // starting from 0 to 2pi

      // vertex position (x, y, z)
      x = xy * std::cos(sectorAngle); // r * cos(u) * cos(v)
      y = xy * std::sin(sectorAngle); // r * cos(u) * sin(v)
      v.pos = glm::vec3{x, y, z};
      v.norm = glm::normalize(v.pos);
      v.uv = {0, 0};

      md->vertices.push_back(v);
    }
  }

  int k1, k2;
  for (uint32_t i = 0; i < stackCount; ++i) {
    k1 = i * (sectorCount + 1); // beginning of current stack
    k2 = k1 + sectorCount + 1;  // beginning of next stack

    for (uint32_t j = 0; j < sectorCount; ++j, ++k1, ++k2) {
      // 2 triangles per sector excluding first and last stacks
      // k1 => k2 => k1+1
      if (i != 0) {
        md->indices.push_back(k1);
        md->indices.push_back(k2);
        md->indices.push_back(k1 + 1);
      }

      // k1+1 => k2 => k2+1
      if (i != (stackCount - 1)) {
        md->indices.push_back(k1 + 1);
        md->indices.push_back(k2);
        md->indices.push_back(k2 + 1);
      }
    }
  }

  if (faceNormals) {
    genFaceNormals(*md);
  }

  meshCache.emplace_back(std::move(md));

  return meshCache.back().get();
}

Mesh *loadMesh(const std::string &) { /* NOT IMPLEMENTED */
  return nullptr;
}

} // namespace fcm
