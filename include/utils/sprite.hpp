#pragma once

#include <utils/stb_image.hpp>

#include <string_view>

namespace fcm {

// not copyable!
class Sprite {
  stbi_image_arr _data;
  int _width, _height, _numChannels;

public:
  explicit Sprite(std::string_view path);
  Sprite(stbi_image_arr data, int width, int height, int numChannels);
  // TODO sprite sheet constructor

  // release memory held in data (if possible)
  void destroy() noexcept;

  int width() const noexcept;
  int height() const noexcept;
  int numChannels() const noexcept;
  unsigned char *data() const noexcept;
};

// a 1x1 sprite of RGBA colour
// (however right now alpha channel is ignored)
Sprite Pixel(unsigned int colour);

} // namespace fcm
