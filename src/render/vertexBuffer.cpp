#include <cstddef>

#include <iostream>
#include <render/vertexBuffer.hpp>
#include <utils/glew.hpp>

namespace fcm {

VertexBuffer::VertexBuffer(const void *data, size_t size) {
  glGenBuffers(1, &handle);
  glBindBuffer(GL_ARRAY_BUFFER, handle);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &handle); }

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, handle); }
void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

} // namespace fcm
