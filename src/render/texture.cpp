#include <render/texture.hpp>
#include <utils/config.hpp>

#include <iostream>

namespace fcm {

Texture::Texture(Sprite sprite) : _sprite{std::move(sprite)} {
  glGenTextures(1, &_handle);
}

Texture::Texture(std::string_view path) : Texture{Sprite{path}} {}

void Texture::bind() const noexcept {
  glActiveTexture(GL_TEXTURE0 + _textureUnit);
  glBindTexture(GL_TEXTURE_2D, _handle);
}

void Texture::to_gpu() noexcept {
  // bind handle
  glBindTexture(GL_TEXTURE_2D, _handle);

  // set wrapping options (repeat is default)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap);
  if (_wrap == GL_CLAMP_TO_BORDER) {
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                     glm::value_ptr(_borderColour));
  }
  // set filtering options (linear is default)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);

  // load to gpu and generate mipmaps
  glTexImage2D(GL_TEXTURE_2D,
               0,           // mipmap level
               _dataFormat, // opengl format
               _sprite.width(), _sprite.height(),
               0,                // legacy arg
               _dataFormat,      // data format
               GL_UNSIGNED_BYTE, // data type
               _sprite.data());
  if (_genMipMaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  // free ram (not necessary but good practice)
  _sprite.destroy();
}

const Sprite &Texture::sprite() const noexcept { return _sprite; }

GLHandle Texture::handle() const noexcept { return _handle; }

void Texture::setTextureUnit(int textureUnit) noexcept {
  // sanity check
  // the actual limit could be greater than 16, but only 16 are guaranteed
  if constexpr (DEBUG_MODE >= 1) {
    if (textureUnit > 15) {
      std::cerr << "[DEBUG] WARNING! texture unit " << textureUnit
                << "prohibited\n";
    }
  }
  _textureUnit = textureUnit;
}

void Texture::setGenMipMaps(bool genMipMaps) noexcept {
  _genMipMaps = genMipMaps;
}

void Texture::setWrap(int wrap) noexcept { _wrap = wrap; }

void Texture::setBorderColour(glm::vec4 colour) noexcept {
  _borderColour = colour;
}

void Texture::setMinFilter(int filter) noexcept { _minFilter = filter; }

void Texture::setMagFilter(int filter) noexcept { _magFilter = filter; }

void Texture::setDataFormat(int format) noexcept { _dataFormat = format; }

} // namespace fcm
