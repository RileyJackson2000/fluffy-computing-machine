#pragma once

#include <cstdint>
#include <string>

namespace fcm {

// controller config
struct Config {

  uint64_t maxTPS = 60;
  uint64_t maxFPS = -1; // no max fps by default (also not implemented yet)

  // can split up other members here such as PhysicsConfig, RenderConfig, etc
  // for now just leave everything in one big config file
  bool faceNormals = true;

  /* window options */
  uint32_t windowWidth = 2048;
  uint32_t windowHeight = 2048;
  std::string windowTitle = "PhysX DLC Features";

  /* output options */
  // TODO I'm putting this here for now but we need a better system for per-run
  // config, it doesnt make sense to recompile to change the output file
  bool record = false;
  std::string outPath = "../out/output";
};

} // namespace fcm
