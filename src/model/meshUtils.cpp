#pragma once

#include <tuple>

#include <utils/glm.hpp>

#include <model/mesh.hpp>

namespace fcm {

float computeVolume(const MeshData &mesh) {
  float totalVolume = 0.f;
  for (size_t i = 0; i < mesh.indices.size(); i += 3) {
    // implicit tet with vertices (0,0,0), v1, v2, v3
    glm::vec3 v1 = mesh.vertices[i + 0].pos;
    glm::vec3 v2 = mesh.vertices[i + 1].pos;
    glm::vec3 v3 = mesh.vertices[i + 2].pos;

    glm::mat3 A{v1, v2, v3};
    float detA = glm::determinant(A);
    float tetVolume = detA;
    totalVolume += tetVolume;
  }
  return totalVolume / 6.f;
}

std::tuple<glm::vec3, glm::mat3, float> computeMassProps(const MeshData &mesh,
                                                         float density) {
  // assume density=1, we will scale everything before returning
  // we start with the covariance of a canonical tet {w0, w1, w2, w3}
  // where w0=(0,0,0), w1=(1,0,0), w2=(0,1,0), w3=(0,0,1)
  glm::mat3 canonicalCovariance{1.f / 60.f,  1.f / 120.f, 1.f / 120.f,
                                1.f / 120.f, 1.f / 60.f,  1.f / 120.f,
                                1.f / 120.f, 1.f / 120.f, 1.f / 60.f};

  glm::mat3 totalCovariance{0.f};
  glm::vec3 centreOfMass{0.f};
  float totalMass{0.f};
  for (size_t i = 0; i < mesh.indices.size(); i += 3) {
    // implicit tet with vertices (0,0,0), v1, v2, v3
    glm::vec3 v1 = mesh.vertices[i + 0].pos;
    glm::vec3 v2 = mesh.vertices[i + 1].pos;
    glm::vec3 v3 = mesh.vertices[i + 2].pos;

    // A is the transformation to convert our canonical tet to implicit tet
    glm::mat3 A{v1, v2, v3};
    float detA = glm::determinant(A);

    glm::mat3 tetCovariance =
        detA * A * canonicalCovariance * glm::transpose(A);
    glm::vec3 tetCOM = v1 + v2 + v3; // this is tetCOM * 4
    float tetMass = detA;            // this is tetVolume * 6

    totalCovariance += tetCovariance;
    centreOfMass += tetMass * tetCOM;
    totalMass += tetMass;
  }

  centreOfMass /= (totalMass * 4.f);
  totalMass /= 6.f;

  // compute the inertia tensor about (0,0,0)
  float trC =
      totalCovariance[0][0] + totalCovariance[1][1] + totalCovariance[2][2];
  glm::mat3 inertiaTensor = glm::mat3{1.f} * trC - totalCovariance;

  // move inertia tensor - parallel axis theorem
  for (size_t c = 0; c < 3; ++c) {
    for (size_t r = 0; r < 3; ++r) {
      float diagFactor = c == r ? glm::dot(centreOfMass, centreOfMass) : 0;
      inertiaTensor[c][r] -=
          totalMass * (diagFactor - centreOfMass[c] * centreOfMass[r]);
    }
  }

  // scale by density
  return {centreOfMass, density * inertiaTensor, density * totalMass};
}

} // namespace fcm
