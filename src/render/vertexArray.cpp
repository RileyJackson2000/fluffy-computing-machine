#include <cstddef>

#include <utils/glew.hpp>

#include <render/vertexArray.hpp>
#include <render/vertexBuffer.hpp>
#include <render/vertexBufferLayout.hpp>

namespace fcm {

VertexArray::VertexArray() { glGenVertexArrays(1, &handle); }
VertexArray::~VertexArray() { glDeleteVertexArrays(1, &handle); }

void VertexArray::addVertexBuffer(const VertexBuffer &vb,
                                  const VertexBufferLayout &layout) {
  bind();
  vb.bind();
  for (size_t i = 0; i < layout.elems.size(); ++i) {
    const auto &elem = layout.elems[i];
    glEnableVertexAttribArray(i);
    // OpenGL is cringe so I had to use reinterpret cast
    glVertexAttribPointer(i, elem.count, elem.type, elem.normalized,
                          layout.numBytes,
                          reinterpret_cast<const void *>(elem.offset));
  }
}

void VertexArray::bind() const { glBindVertexArray(handle); }

void VertexArray::unbind() const { glBindVertexArray(0); }

} // namespace fcm
