#pragma once

#include <utils/glew.hpp>

namespace fcm {
    
struct IndexBuffer
{
    glew::GLHandle handle;
    const unsigned int numIndices;

    IndexBuffer(const glew::GLuint* data, unsigned int numIndices);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
};

} // namespace fcm
