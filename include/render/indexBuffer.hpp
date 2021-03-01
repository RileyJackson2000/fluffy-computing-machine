#pragma once

#include <cstddef>

#include <utils/glew.hpp>

namespace fcm {

struct IndexBuffer {
  GLHandle handle;
  const size_t numIndices;

  IndexBuffer(const GLuint *data, size_t numIndices);
  // non-copyable, non-movable
  // this prevents annoying opengl issues with the handle
  // see GLMesh.hpp for more details
  IndexBuffer(const IndexBuffer &) = delete;
  IndexBuffer(IndexBuffer &&) = delete;
  IndexBuffer &operator=(IndexBuffer) = delete;
  ~IndexBuffer();

  void bind() const;
  void unbind() const;
};

} // namespace fcm
