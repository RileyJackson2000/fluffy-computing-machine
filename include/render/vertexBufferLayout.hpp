#pragma once

#include <type_traits>
#include <vector>

#include <utils/glew.hpp>

namespace fcm {

template <class T> struct dependent_false : std::false_type {};

template <typename T> constexpr GLenum getTypeEnum() {
  if constexpr (std::is_same<T, GLuint>::value ||
                std::is_same<T, unsigned int>::value)
    return GL_UNSIGNED_INT;
  else if constexpr (std::is_same<T, GLfloat>::value ||
                     std::is_same<T, float>::value)
    return GL_FLOAT;
  else
    static_assert(dependent_false<T>::value,
                  "Unrecognized type in fcm::vertexBufferLayout::getTypeEnum");
}

struct VertexBufferLayoutElem {
  GLenum type;
  size_t count;
  GLint normalized;

  size_t offset;
};

struct VertexBufferLayout {
  std::vector<VertexBufferLayoutElem> elems;
  size_t numBytes; // stride

  VertexBufferLayout() : numBytes{0} {}
  ~VertexBufferLayout() {}

  template <typename T> void addElem(size_t count, bool normalized = false) {
    // struct packing issues possible?
    elems.push_back(
        {getTypeEnum<T>(), count, normalized ? GL_TRUE : GL_FALSE, numBytes});
    numBytes += count * sizeof(T);
  }
};

} // namespace fcm
