#pragma once

#include <vector>

namespace fcm {

// controller config
struct Config {

  size_t maxTPS = 60;
  size_t maxFPS = -1; // no max fps by default (also not implemented yet)

  // can split up other members here such as PhysicsConfig, RenderConfig, etc
  // for now just leave everything in one big config file
  bool faceNormals = true;
};

} // namespace fcm
