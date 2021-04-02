#include <utils/image.hpp>

namespace fcm {

Image::Image(const std::string &path) {
  auto raw_data_ptr =
      stbi_load(path.c_str(), &_width, &_height, &_numChannels, 0);

  if (raw_data_ptr == nullptr) {
    throw std::runtime_error{"Failed to load sprite " + path};
  }

  _data = stbi_image_arr{raw_data_ptr};
}

Image::Image(stbi_image_arr data, int width, int height, int numChannels)
    : _data{std::move(data)}, _width{width}, _height{height},
      _numChannels{numChannels} {}

void Image::destroy() noexcept { _data.reset(); }

int Image::width() const noexcept { return _width; }

int Image::height() const noexcept { return _height; }

int Image::numChannels() const noexcept { return _numChannels; }

unsigned char *Image::data() const noexcept { return _data.get(); }

Image Pixel(Colour colour) {
  constexpr int width = 1;
  constexpr int height = 1;
  constexpr int numChannels = 3;
  stbi_image_arr data = stbi_new_image_arr(width, height, numChannels);
  data[0] = colour.red();
  data[1] = colour.green();
  data[2] = colour.blue();
  return Image{std::move(data), width, height, numChannels};
}

Image NullImage(int width, int height, int numChannels) {
  return Image{stbi_image_arr{}, width, height, numChannels};
}

} // namespace fcm
