#pragma once

#include <utils/colour.hpp>
#include <utils/stb_image.hpp>

#include <string_view>

namespace fcm {

// not copyable!
class Image {
  stbi_image_arr _data;
  int _width, _height, _numChannels;

public:
  explicit Image(const std::string &path);
  Image(stbi_image_arr data, int width, int height, int numChannels);
  // TODO sprite sheet constructor

  // release memory held in data (if possible)
  void destroy() noexcept;

  int width() const noexcept;
  int height() const noexcept;
  int numChannels() const noexcept;
  unsigned char *data() const noexcept;
};

// a 1x1 sprite of specified colour
// (alpha channel is ignored for now)
Image Pixel(Colour);

Image NullImage(int width, int height, int numChannels);

} // namespace fcm
