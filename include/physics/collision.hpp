#pragma once

#include <model/material.hpp>
#include <model/object.hpp>
#include <utils/glm.hpp>

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <optional>
#include <vector>

namespace fcm {

glm::vec3 friction_dir(const glm::vec3 &v, const glm::vec3 &n);

void collide(Sphere &s1, Sphere &s2);

void collide(Mesh &m1, Mesh &m2);

void collide(Sphere &s, Mesh &m);

inline void collide(Mesh &m, Sphere &s);

namespace {
template <typename T> void collidesWith(T &obj_1, Object &obj_2);
}

void collide(Object &obj_1, Object &obj_2);

} // namespace fcm
