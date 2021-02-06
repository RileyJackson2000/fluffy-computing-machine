#pragma once

#include <utils/glew.hpp>

#include <render/vertexBuffer.hpp>
#include <render/vertexBufferLayout.hpp>

namespace fcm {

struct VertexArray {
  glew::GLHandle handle;

  VertexArray();
  ~VertexArray();

  void addVertexBuffer(const VertexBuffer &vb,
                       const VertexBufferLayout &layout);
  void bind() const;
  void unbind() const;
};

} // namespace fcm
