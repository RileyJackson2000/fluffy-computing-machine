#pragma once

#include <utils/glew.hpp>

namespace fcm {
    
struct IndexBuffer
{
    glew::GLHandle handle;
    const unsigned int numIndices;

    IndexBuffer(const glew::GLuint* data, unsigned int numIndices)
    :   numIndices{numIndices}
    {
        glew::glGenBuffers(1, &handle);
        glew::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
        glew::glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(glew::GLuint), data, GL_STATIC_DRAW);
    }
    ~IndexBuffer()
    {
        glew::glDeleteBuffers(1, &handle);
    }

    void bind() const
    {
        glew::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    }
    void unbind() const
    {
        glew::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};

} // namespace fcm
