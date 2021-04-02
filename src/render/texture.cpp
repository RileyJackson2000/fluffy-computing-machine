#include <render/texture.hpp>
#include <utils/config.hpp>

#include <iostream>

namespace fcm {

Texture::Texture(Image image) : _image{std::move(image)} {
  glGenTextures(1, &_handle);
}

Texture::Texture(const std::string &path) : Texture{Image{path}} {}

void Texture::bind() const noexcept { glBindTexture(GL_TEXTURE_2D, _handle); }

void Texture::to_gpu() noexcept {
  if constexpr (debug_mode > 0) {
    if (_image.data() == nullptr) {
      std::cerr << "Image data is null?\n";
      return;
    }
  }

  // bind handle
  glBindTexture(GL_TEXTURE_2D, _handle);

  // set wrapping options (repeat for now as default)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set filtering options (linear for now as default)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load to gpu and generate mipmaps
  glTexImage2D(GL_TEXTURE_2D,
               0,      // mipmap level
               GL_RGB, // opengl format
               _image.width(), _image.height(),
               0,                // legacy arg
               GL_RGB,           // data format
               GL_UNSIGNED_BYTE, // data type
               _image.data());
  glGenerateMipmap(GL_TEXTURE_2D);

  // free ram (not necessary but good practice)
  _image.destroy();
}

const Image &Texture::image() const noexcept { return _image; }

GLHandle Texture::handle() const noexcept { return _handle; }

} // namespace fcm
