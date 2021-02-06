#pragma once

#include <cstddef>

#include <utils/glew.hpp>

namespace fcm {
    
struct IndexBuffer
{
    glew::GLHandle handle;
    const size_t numIndices;

    IndexBuffer(const glew::GLuint* data, size_t numIndices);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
};

} // namespace fcm
