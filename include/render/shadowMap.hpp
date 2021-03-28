#pragma once

#include <model/light.hpp>
#include <render/texture.hpp>
#include <utils/glfw.hpp>

namespace fcm {

class ShadowMap {
  Texture _depthMap;
  int _width, _height; // resolution
  GLHandle _depthMapFBO;

public:
  ShadowMap(int width, int height);

  void bind() const noexcept;
  void bindFrameBuffer() const noexcept;
  void bindTexture() const noexcept;
  void to_gpu() noexcept;

  void setTextureUnit(int) noexcept;

  int width() const noexcept;
  int height() const noexcept;
  GLHandle depthMap() const noexcept;
  GLHandle depthMapFBO() const noexcept;
};
} // namespace fcm
