#include <string>
#include <utils/config.hpp>

namespace fcm {

uint64_t Config::maxTPS = 60;
uint64_t Config::maxFPS = -1;

bool Config::faceNormals = true;

/* window options */
uint32_t Config::windowWidth = 720;
uint32_t Config::windowHeight = 540;

// this is such a stupid language rule
std::string Config::windowTitle = "Title goes here";

} // namespace fcm
