#include <gtest/gtest.h>

TEST(FluffySuite, HelloWorldTest) { EXPECT_EQ(1, 1); }

#include <tuple>

#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <model/meshUtils.hpp>

void sphereTest() {
  float density = 3.f;
  float radius = 2;
  auto sphere = fcm::genSphereMesh(radius, 100, 100);
  glm::vec3 sphereCOM{10.f, -20.f, 0.f};
  fcm::transformMeshPoints(*sphere.get(),
                           glm::translate(glm::mat4{1.f}, sphereCOM));

  auto massProps = fcm::computeMassProps(*sphere.get(), density);

  float expectedMass = density * (4.f * M_PI * radius * radius * radius / 3.f);
  glm::vec3 expectedCOM = sphereCOM;
  glm::mat3 expectedInertia{2.f * expectedMass * radius * radius / 5.f};
}

void cubeTest() {
  float density = 5.f;
  float halfSide = 5;
  auto cube = fcm::genCubeMesh(halfSide);
  glm::vec3 cubeCOM{0.f, -1.f, 2};
  fcm::transformMeshPoints(*cube.get(),
                           glm::translate(glm::mat4{1.f}, cubeCOM));

  auto massProps = fcm::computeMassProps(*cube.get(), density);

  float side = 2.f * halfSide;

  float expectedMass = density * (side * side * side);
  glm::vec3 expectedCOM = cubeCOM;
  glm::mat3 expectedInertia{expectedMass * side * side / 6.f};
}
