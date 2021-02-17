#include <model/object.hpp>
#include <physics/collision.hpp>
#include <utils/glm.hpp>

namespace fcm {

glm::vec3 friction_dir(const glm::vec3 &v, const glm::vec3 &n) {
  // we want to solve for the frictional force f
  // here, v is velocity vector and n is normal of plane
  // v - proj(v, n) is the projection of v onto the plane with normal n
  // we make it negative because friction force opposes direction of movement
  // then we normalize it to get a direction
  // TODO this divides by zero if velocity is perpendicular to normal
  return glm::normalize(glm::proj(v, n) - v);
}

void collide(Sphere &s1, Sphere &s2) {
  const auto p1 = s1.position;
  const auto p2 = s2.position;

  const float d = glm::distance(p1, p2);
  const float r1 = s1.radius;
  const float r2 = s2.radius;
  if (d > r1 + r2) {
    // no collision
    return;
  }
  // amount the spheres are compressed by
  const float x = r1 + r2 - d;

  const auto k1 = spring_constant(s1.mat);
  const auto k2 = spring_constant(s2.mat);

  // distance each sphere is compressed by
  // assume proportional to their spring constants
  const float x1 = x * k2 / (k1 + k2);
  const float x2 = x * k1 / (k1 + k2);

  // spring forces (sum of individual spring forces)
  const float spring_mag = k1 * x1 + k2 * x2;
  // normalized radius vector pointing out of 1
  // TODO this divides by 0 if p1 == p2!
  const glm::vec3 rad1 = glm::normalize(p2 - p1);
  const glm::vec3 rad2 = -rad1;
  const glm::vec3 force_1 = spring_mag * rad2;
  s1.force += force_1;

  const glm::vec3 force_2 = spring_mag * rad1;
  s2.force += force_2;

  // calculate tangential friction forces
  const float mu = static_friction(s1.mat, s2.mat);

  const glm::vec3 fforce_1 =
      mu * spring_mag * friction_dir(s1.velocity, force_1);
  const glm::vec3 fforce_2 =
      mu * spring_mag * friction_dir(s2.velocity, force_1);

  // convert to torque
  // torque = r x F
  const glm::dvec3 torque_1 = glm::cross((r1 - x1) * rad1, fforce_1);
  const glm::dvec3 torque_2 = glm::cross((r2 - x2) * rad2, fforce_2);
  // this is def wrong

  s1.torque += torque_1;
  s2.torque += torque_2;
}

void collide(Mesh &m1, Mesh &m2) {
  (void)m1;
  (void)m2;
}

void collide(Sphere &s, Mesh &m) {
  (void)s;
  (void)m;
}

inline void collide(Mesh &m, Sphere &s) { collide(s, m); }

namespace {
template <typename T> void collidesWith(T &obj_1, Object &obj_2) {
  switch (obj_2.objectType) {
  case ObjectType::SPHERE:
    collide(obj_1, static_cast<Sphere &>(obj_2));
    return;
  case ObjectType::MESH:
    collide(obj_1, static_cast<Mesh &>(obj_2));
    return;
  default:
    return;
  }
}
} // namespace

void collide(Object &obj_1, Object &obj_2) {
  switch (obj_1.objectType) {
  case ObjectType::SPHERE:
    collidesWith(static_cast<Sphere &>(obj_1), obj_2);
    return;
  case ObjectType::MESH:
    collidesWith(static_cast<Mesh &>(obj_1), obj_2);
    return;
  default:
    return;
  }
}

} // namespace fcm
