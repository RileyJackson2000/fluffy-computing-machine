#pragma once

#include <model/scene.hpp>

namespace fcm {

void init_physics(void);

void update(Scene &, float dt);

} // namespace fcm
