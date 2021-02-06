#include <cstddef>

#include <utils/glew.hpp>

#include <render/indexBuffer.hpp>

namespace fcm {

IndexBuffer::IndexBuffer(const glew::GLuint *data, size_t numIndices)
    : numIndices{numIndices} {
  glew::glGenBuffers(1, &handle);
  glew::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
  glew::glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(glew::GLuint),
                     data, GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer() { glew::glDeleteBuffers(1, &handle); }

void IndexBuffer::bind() const {
  glew::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}
void IndexBuffer::unbind() const {
  glew::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace fcm
