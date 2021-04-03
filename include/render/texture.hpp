#pragma once

#include <utils/glfw.hpp>
#include <utils/image.hpp>

namespace fcm {

class Texture {
  Image _image;
  GLHandle _handle;

public:
  explicit Texture(Image);
  explicit Texture(const std::string &path);

  void bind() const noexcept;
  // moves the image data to gpu
  // don't call this more than once as it destroys the cpu copy
  void to_gpu() noexcept;

  const Image &image() const noexcept;
  GLHandle handle() const noexcept;
};
} // namespace fcm
