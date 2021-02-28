#pragma once

#include <cstdint>
#include <string>

// if DEBUG_MODE is set to 1, then certain runtime checks and logs are enabled
// to try and prematurely catch bugs
// set to 0 for production to maximize performance at the cost of allowing some
// systems to fail silently
#define DEBUG_MODE 1

namespace fcm {

inline constexpr int debug_mode = DEBUG_MODE;

// controller config
inline struct Config {
  uint64_t maxTPS = 60;
  uint64_t maxFPS = -1; // no max fps by default (also not implemented yet)

  // can split up other members here such as PhysicsConfig, RenderConfig, etc
  // for now just leave everything in one big config file
  bool faceNormals = true;

  /* window options */
  uint32_t windowWidth = 960;
  uint32_t windowHeight = 640;
  std::string windowTitle = "PhysX DLC Features";

  /* output options */
  // TODO I'm putting this here for now but we need a better system for per-run
  // config, it doesnt make sense to recompile to change the output file
  bool record = false;
  std::string outPath = "../out/output";
} config;

} // namespace fcm
