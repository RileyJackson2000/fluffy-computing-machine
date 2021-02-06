#include <cstddef>

#include <utils/glew.hpp>

#include <render/vertexArray.hpp>
#include <render/vertexBuffer.hpp>
#include <render/vertexBufferLayout.hpp>

namespace fcm {

VertexArray::VertexArray() { glew::glGenVertexArrays(1, &handle); }
VertexArray::~VertexArray() { glew::glDeleteVertexArrays(1, &handle); }

void VertexArray::addVertexBuffer(const VertexBuffer &vb,
                                  const VertexBufferLayout &layout) {
  bind();
  vb.bind();
  for (size_t i = 0; i < layout.elems.size(); ++i) {
    const auto &elem = layout.elems[i];
    glew::glEnableVertexAttribArray(i);
    // OpenGL is cringe so I had to use reinterpret cast
    glew::glVertexAttribPointer(i, elem.count, elem.type, elem.normalized,
                                layout.numBytes,
                                reinterpret_cast<const void *>(elem.offset));
  }
}

void VertexArray::bind() const { glew::glBindVertexArray(handle); }

void VertexArray::unbind() const { glew::glBindVertexArray(0); }

} // namespace fcm
