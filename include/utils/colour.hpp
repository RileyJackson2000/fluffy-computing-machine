#pragma once

namespace fcm {

// Canadian approved colour class
// lots more features tbd
struct Colour {
  // use single letters to be consistent with glsl
  float r, g, b, a;

  Colour(float red, float green, float blue);
  Colour(float red, float green, float blue, float alpha);
  Colour(int red, int green, int blue, int alpha);

  unsigned int hexRGBA() const noexcept;

  // getters to return rgba ints in the range [0, 255]
  unsigned char red() const noexcept;
  unsigned char green() const noexcept;
  unsigned char blue() const noexcept;
  unsigned char alpha() const noexcept;
};

// factory functions
Colour fromRGBA(unsigned int rgba_hexcode);
Colour fromARGB(unsigned int argb_hexcode);
Colour fromRGB(unsigned int rgb_hexcode);
Colour fromHSV(unsigned int hsv_hexcode);

} // namespace fcm
