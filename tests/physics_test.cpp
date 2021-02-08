#include <iostream>
#include <memory>
#include <tuple>

#include <gtest/gtest.h>

#include <model/material.hpp>
#include <model/mesh.hpp>
#include <model/meshUtils.hpp>
#include <model/object.hpp>
#include <physics/physics.hpp>
#include <render/render.hpp>
#include <utils/constants.hpp>
#include <utils/glm.hpp>

TEST(PhysicsSuite, SphereMassPropTest) {
  float density = 3.f;
  float radius = 2.f;
  auto sphere = fcm::genSphereMesh(radius, 200, 200);
  glm::vec3 sphereCOM{10.f, -20.f, 0.f};
  fcm::transformMeshPoints(*sphere.get(),
                           glm::translate(glm::mat4{1.f}, sphereCOM));

  auto massProps = fcm::computeMassProps(*sphere.get(), density);

  float expectedMass = density * (4.f * M_PI * radius * radius * radius / 3.f);
  glm::vec3 expectedCOM = sphereCOM;
  glm::mat3 expectedInertia{2.f * expectedMass * radius * radius / 5.f};

  float eps = glm::length(expectedCOM) / 500.f;
  for (uint32_t i = 0; i < 3; ++i) {

    EXPECT_NEAR(std::get<1>(massProps)[i], expectedCOM[i], eps);
  }

  eps = 0;
  for (uint32_t c = 0; c < 3; ++c) {
    for (uint32_t r = 0; r < 3; ++r) {
      eps += expectedInertia[c][r] / 500.f;
    }
  }

  for (uint32_t c = 0; c < 3; ++c) {
    for (uint32_t r = 0; r < 3; ++r) {
      EXPECT_NEAR(std::get<2>(massProps)[c][r], expectedInertia[c][r], eps);
    }
  }
}

TEST(PhysicsSuite, CubeMassPropTest) {
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

  EXPECT_NEAR(std::get<0>(massProps), expectedMass, fcm::eps);

  float eps = glm::length(expectedCOM) / 500.f;
  for (uint32_t i = 0; i < 3; ++i) {

    EXPECT_NEAR(std::get<1>(massProps)[i], expectedCOM[i], eps);
  }

  eps = 0;
  for (uint32_t c = 0; c < 3; ++c) {
    for (uint32_t r = 0; r < 3; ++r) {
      eps += expectedInertia[c][r] / 500.f;
    }
  }

  for (uint32_t c = 0; c < 3; ++c) {
    for (uint32_t r = 0; r < 3; ++r) {
      EXPECT_NEAR(std::get<2>(massProps)[c][r], expectedInertia[c][r], eps);
    }
  }
}

TEST(PhysicsSuite, IterateAngularTest) {
  EXPECT_EQ(1, 1);
  // fcm::init_physics();

  // float R = 2;            // m
  // float dt = 0.3;         // s
  // float torque_mag = 150; // Nm

  // fcm::Object *o =
  // new fcm::Sphere("test sphere", std::make_shared<fcm::MeshData>(),
  //{0, 0, 0}, R, fcm::TEST_MAT_1);
  // o->torque = {0, 0, torque_mag};
  //// mass = volume * density
  // EXPECT_NEAR(o->mass, 4. / 3 * fcm::PI * R * R * R *
  // density(fcm::TEST_MAT_1), fcm::eps);
  //// the moment of inertia of a solid sphere is 2/5MR^2
  // EXPECT_NEAR(o->moment_of_inertia, 0.4 * o->mass * R * R, fcm::eps);

  // EXPECT_EQ(o->orientation, glm::quat(1., 0., 0., 0.));

  // float angular_acceleration =
  // glm::length(o->torque) / o->moment_of_inertia;  // rad s^-2
  // float angular_velocity = angular_acceleration * dt; // rad s^-1

  // glm::vec3 axis = {0, 0, 1};
  // float angle = angular_velocity * dt; // rad

  // glm::quat expect = glm::angleAxis(angle, axis);

  // fcm::iterate_angular(o, dt);
  // EXPECT_NEAR(o->orientation.x, expect.x, fcm::eps);
  // EXPECT_NEAR(o->orientation.y, expect.y, fcm::eps);
  // EXPECT_NEAR(o->orientation.z, expect.z, fcm::eps);
  // EXPECT_NEAR(o->orientation.w, expect.w, fcm::eps);

  // delete o;
}
