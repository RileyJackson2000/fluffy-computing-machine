#pragma once

#include <utils/glfw.hpp>
#include <utils/sprite.hpp>

namespace fcm {

class Texture {
  Sprite _sprite;
  GLHandle _handle;

public:
  explicit Texture(Sprite);
  explicit Texture(std::string_view path);

  void bind() const noexcept;
  // moves the sprite data to gpu
  // don't call this more than once as it destroys the cpu copy
  void to_gpu() noexcept;

  const Sprite &sprite() const noexcept;
  GLHandle handle() const noexcept;
};
} // namespace fcm
