#include <gtest/gtest.h>
#include <memory>

#include <model/material.hpp>
#include <model/mesh.hpp>
#include <model/object.hpp>
#include <physics/physics.hpp>
#include <render/render.hpp>
#include <utils/constants.hpp>
#include <utils/glm.hpp>

TEST(PhysicsSuite, IterateAngularTest) {
  fcm::init_physics();
  fcm::Viewer viewer{};

  float R = 2;            // m
  float dt = 0.3;         // s
  float torque_mag = 150; // Nm

  fcm::Object *o =
      new fcm::Sphere("test sphere", fcm::genSphereMesh(R, 10, 10, true),
                      {0, 0, 0}, R, fcm::TEST_MAT_1);
  o->torque = {0, 0, torque_mag};
  // mass = volume * density
  EXPECT_NEAR(o->mass, 4. / 3 * fcm::PI * R * R * R * density(fcm::TEST_MAT_1),
              fcm::eps);
  // the moment of inertia of a solid sphere is 2/5MR^2
  EXPECT_NEAR(o->moment_of_inertia, 0.4 * o->mass * R * R, fcm::eps);

  EXPECT_EQ(o->orientation, glm::quat(1., 0., 0., 0.));

  float angular_acceleration =
      glm::length(o->torque) / o->moment_of_inertia;  // rad s^-2
  float angular_velocity = angular_acceleration * dt; // rad s^-1

  glm::vec3 axis = {0, 0, 1};
  float angle = angular_velocity * dt; // rad

  glm::quat expect = glm::angleAxis(angle, axis);

  fcm::iterate_angular(o, dt);
  EXPECT_NEAR(o->orientation.x, expect.x, fcm::eps);
  EXPECT_NEAR(o->orientation.y, expect.y, fcm::eps);
  EXPECT_NEAR(o->orientation.z, expect.z, fcm::eps);
  EXPECT_NEAR(o->orientation.w, expect.w, fcm::eps);

  delete o;
}
