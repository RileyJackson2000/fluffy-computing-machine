#include <cstddef>

#include <utils/glew.hpp>

#include <render/indexBuffer.hpp>

namespace fcm {

IndexBuffer::IndexBuffer(const GLuint *data, size_t numIndices)
    : numIndices{numIndices} {
  glGenBuffers(1, &handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), data,
               GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &handle); }

void IndexBuffer::bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}
void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

} // namespace fcm
