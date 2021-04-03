#include <utils/colour.hpp>

namespace fcm {

Colour::Colour(float red, float green, float blue)
    : Colour{red, green, blue, 1.f} {}

Colour::Colour(float red, float green, float blue, float alpha)
    : r{red}, g{green}, b{blue}, a{alpha} {}

Colour::Colour(int red, int green, int blue, int alpha)
    : Colour{red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f} {}

unsigned int Colour::hexRGBA() const noexcept {
  return (red() << 24) + (blue() << 16) + (green() << 8) + alpha();
}

inline unsigned char _to255(float f) {
  // round to nearest integer
  return static_cast<unsigned char>(f * 255 + 0.5);
}

unsigned char Colour::red() const noexcept { return _to255(r); }

unsigned char Colour::green() const noexcept { return _to255(g); }

unsigned char Colour::blue() const noexcept { return _to255(b); }

unsigned char Colour::alpha() const noexcept { return _to255(a); }

Colour fromRGBA(unsigned int hexcode) {
  int red = (hexcode & 0xff000000) >> 24;
  int green = (hexcode & 0x00ff0000) >> 16;
  int blue = (hexcode & 0x0000ff00) >> 8;
  int alpha = (hexcode & 0x000000ff);
  return Colour(red, green, blue, alpha);
}

Colour fromARGB(unsigned int hexcode) {
  int alpha = (hexcode & 0xff000000) >> 24;
  int red = (hexcode & 0x00ff0000) >> 16;
  int green = (hexcode & 0x0000ff00) >> 8;
  int blue = (hexcode & 0x000000ff);
  return Colour(red, green, blue, alpha);
}

Colour fromRGB(unsigned int hexcode) {
  int red = (hexcode & 0x00ff0000) >> 16;
  int green = (hexcode & 0x0000ff00) >> 8;
  int blue = (hexcode & 0x000000ff);
  return Colour(red, green, blue, 255);
}

Colour fromHSV(unsigned int hexcode) {
  (void)hexcode;
  throw "not implemented";
}

} // namespace fcm
