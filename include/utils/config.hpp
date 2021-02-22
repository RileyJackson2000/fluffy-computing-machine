#pragma once

#include <cstdint>
#include <string>

namespace fcm {

// controller config
struct Config {

  static uint64_t maxTPS;
  static uint64_t maxFPS;

  // can split up other members here such as PhysicsConfig, RenderConfig, etc
  // for now just leave everything in one big config file
  static bool faceNormals;

  /* window options */
  static uint32_t windowWidth;
  static uint32_t windowHeight;
  static std::string windowTitle;
};

} // namespace fcm
