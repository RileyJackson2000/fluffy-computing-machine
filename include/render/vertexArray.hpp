#pragma once

#include <utils/glew.hpp>

#include <render/vertexBuffer.hpp>
#include <render/vertexBufferLayout.hpp>

namespace fcm {

struct VertexArray {
  GLHandle handle;

  VertexArray();
  // non-copyable, non-movable
  // this prevents annoying opengl issues with the handle
  // see GLMesh.hpp for more details
  VertexArray(const VertexArray &) = delete;
  VertexArray(VertexArray &&) = delete;
  VertexArray &operator=(VertexArray) = delete;
  ~VertexArray();

  void addVertexBuffer(const VertexBuffer &vb,
                       const VertexBufferLayout &layout);
  void bind() const;
  void unbind() const;
};

} // namespace fcm
