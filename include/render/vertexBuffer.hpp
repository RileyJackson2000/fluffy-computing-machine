#pragma once

#include <cstddef>

#include <utils/glew.hpp>

namespace fcm {

struct VertexBuffer {
  glew::GLHandle handle;

  VertexBuffer(const void *data, size_t size);
  // non-copyable, non-movable
  // this prevents annoying opengl issues with the handle
  // see GLMesh.hpp for more details
  VertexBuffer(const VertexBuffer &) = delete;
  VertexBuffer(VertexBuffer &&) = delete;
  VertexBuffer &operator=(VertexBuffer) = delete;
  ~VertexBuffer();

  void bind() const;
  void unbind() const;
};

} // namespace fcm
