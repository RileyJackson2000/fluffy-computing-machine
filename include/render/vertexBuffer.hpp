#pragma once

#include <utils/glew.hpp>

namespace fcm {
    
struct VertexBuffer
{
    glew::GLHandle handle;

    VertexBuffer(const void* data, unsigned int size)
    {
        glew::glGenBuffers(1, &handle);
        glew::glBindBuffer(GL_ARRAY_BUFFER, handle);
        glew::glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    ~VertexBuffer()
    {
        glew::glDeleteBuffers(1, &handle);
    }

    void bind() const
    {
        glew::glBindBuffer(GL_ARRAY_BUFFER, handle);
    }
    void unbind() const
    {
        glew::glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

} // namespace fcm
