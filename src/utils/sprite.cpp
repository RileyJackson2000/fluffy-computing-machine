#include <utils/sprite.hpp>

namespace fcm {

Sprite::Sprite(std::string_view path) {
  auto raw_data_ptr =
      stbi_load(path.data(), &_width, &_height, &_numChannels, 0);

  if (raw_data_ptr == nullptr) {
    // no std::string + std::string_view overload for some reason
    throw std::runtime_error{std::string{"Failed to load sprite "} +
                             path.data()};
  }

  _data = stbi_image_arr{raw_data_ptr};
}

Sprite::Sprite(stbi_image_arr data, int width, int height, int numChannels)
    : _data{std::move(data)}, _width{width}, _height{height},
      _numChannels{numChannels} {}

void Sprite::destroy() noexcept { _data.reset(); }

int Sprite::width() const noexcept { return _width; }

int Sprite::height() const noexcept { return _height; }

int Sprite::numChannels() const noexcept { return _numChannels; }

unsigned char *Sprite::data() const noexcept { return _data.get(); }

Sprite Pixel(unsigned int colour) {
  unsigned char *data = static_cast<unsigned char *>(STBI_MALLOC(3));
  if (data == nullptr) {
    throw std::runtime_error{"unable to allocate memory???"};
  }
  data[0] = (colour & 0xff000000) >> 24;
  data[1] = (colour & 0x00ff0000) >> 16;
  data[2] = (colour & 0x0000ff00) >> 8;
  return Sprite{stbi_image_arr{data}, 1, 1, 3};
}

Sprite NullSprite(int width, int height, int numChannels) {
  return Sprite{stbi_image_arr{}, width, height, numChannels};
}

} // namespace fcm
