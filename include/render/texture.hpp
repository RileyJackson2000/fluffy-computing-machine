#pragma once

#include <utils/glfw.hpp>
#include <utils/sprite.hpp>

namespace fcm {

class Texture {
  Sprite _sprite;
  GLHandle _handle;

  // opengl options (usually default is good enough)
  // if you want more fine-grained control over how your texture is rendered
  // you can change these below
  bool _genMipMaps{true};
  int _textureUnit{0};
  int _wrap{GL_REPEAT};
  glm::vec4 _borderColour{1.f, 1.f, 1.f, 1.f};
  int _minFilter{GL_LINEAR};
  int _magFilter{GL_LINEAR};
  int _dataFormat{GL_RGB};

public:
  explicit Texture(Sprite);
  explicit Texture(std::string_view path);

  void bind() const noexcept;
  // moves the sprite data to gpu
  // don't call this more than once as it destroys the cpu copy
  void to_gpu() noexcept;

  const Sprite &sprite() const noexcept;
  GLHandle handle() const noexcept;

  /* set opengl options */
  void setTextureUnit(int) noexcept;

  void setGenMipMaps(bool) noexcept;

  // set the wrapping option
  // eg: GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
  void setWrap(int wrap) noexcept;
  // for GL_CLAMP_TO_BORDER we need to specify a colour (in rgba)
  void setBorderColour(glm::vec4 colour) noexcept;

  // set the filtering option for minifying/magnifying
  // eg: GL_NEAREST, GL_LINEAR. Mipmaps aren't supported yet
  void setMinFilter(int filter) noexcept;
  void setMagFilter(int filter) noexcept;

  // set the data format
  // eg: GL_RGB, GL_RGBA, GL_RGB_INTEGER, etc
  void setDataFormat(int format) noexcept;
};
} // namespace fcm
