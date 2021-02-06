#pragma once

#include <cstddef>

#include <utils/glew.hpp>

namespace fcm {

struct VertexBuffer {
  glew::GLHandle handle;

  VertexBuffer(const void *data, size_t size);
  ~VertexBuffer();

  void bind() const;
  void unbind() const;
};

} // namespace fcm
