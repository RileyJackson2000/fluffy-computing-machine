#include <model/mesh.hpp>

namespace fcm {

void MeshData::faceNormals() {
  std::vector<Vertex> tempVerts;
  tempVerts.reserve(indices.size());

  std::vector<uint32_t> tempInds;
  tempInds.reserve(indices.size());

  for (uint32_t i = 0; i < indices.size(); i += 3) {
    glm::vec3 a = vertices[indices[i + 0]].pos;
    glm::vec3 b = vertices[indices[i + 1]].pos;
    glm::vec3 c = vertices[indices[i + 2]].pos;
    glm::vec3 surfaceNorm = glm::normalize(glm::cross(b - a, c - a));

    Vertex a_new = {a, surfaceNorm};
    Vertex b_new = {b, surfaceNorm};
    Vertex c_new = {c, surfaceNorm};

    tempVerts.push_back(a_new);
    tempVerts.push_back(b_new);
    tempVerts.push_back(c_new);

    tempInds.push_back(i + 0);
    tempInds.push_back(i + 1);
    tempInds.push_back(i + 2);
  }

  vertices.clear();
  indices.clear();

  vertices = tempVerts;
  indices = tempInds;
}

MeshData genSphereMesh(float radius, uint32_t sectorCount, uint32_t stackCount,
                       bool faceNormals = true) {
  MeshData md;

  float x, y, z, xy;                           // vertex position
  float nx, ny, nz, lengthInv = 1.0f / radius; // vertex normal

  float sectorStep = 2.0 * M_PI / float(sectorCount);
  float stackStep = M_PI / float(stackCount);
  float sectorAngle, stackAngle;

  for (uint32_t i = 0; i <= stackCount; ++i) {
    stackAngle = M_PI / 2.0 - i * stackStep; // starting from pi/2 to -pi/2
    xy = radius * std::cos(stackAngle);      // r * cos(u)
    z = radius * std::sin(stackAngle);       // r * sin(u)

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

      // normalized vertex normal (nx, ny, nz)
      nx = x * lengthInv;
      ny = y * lengthInv;
      nz = z * lengthInv;
      v.norm = glm::vec3{nx, ny, nz};

      md.vertices.push_back(v);
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
        md.indices.push_back(k1);
        md.indices.push_back(k2);
        md.indices.push_back(k1 + 1);
      }

      // k1+1 => k2 => k2+1
      if (i != (stackCount - 1)) {
        md.indices.push_back(k1 + 1);
        md.indices.push_back(k2);
        md.indices.push_back(k2 + 1);
      }
    }
  }

  if (faceNormals)
    md.faceNormals();

  return md;
}

} // namespace fcm
