#pragma once

#include <model/scene.hpp>

namespace fcm {

void init_physics(void);

void update(Scene *, float dt);

void iterate_linear(Object *, float dt);
void iterate_angular(Object *, float dt);

} // namespace fcm
