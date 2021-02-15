#pragma once

#include <cstdint>

namespace fcm {

// controller config
struct Config {

  uint64_t maxTPS = 60;
  uint64_t maxFPS = -1; // no max fps by default (also not implemented yet)

  // can split up other members here such as PhysicsConfig, RenderConfig, etc
  // for now just leave everything in one big config file
  bool faceNormals = true;

  /* window options */
  uint32_t windowWidth = 720;
  uint32_t windowHeight = 540;
  const char *windowTitle = "Title goes here";
};

} // namespace fcm
