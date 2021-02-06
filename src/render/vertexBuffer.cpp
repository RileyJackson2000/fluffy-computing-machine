#pragma once

#include <utils/glew.hpp>
#include <render/vertexBuffer.hpp>

namespace fcm {

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glew::glGenBuffers(1, &handle);
    glew::glBindBuffer(GL_ARRAY_BUFFER, handle);
    glew::glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
VertexBuffer::~VertexBuffer()
{
    glew::glDeleteBuffers(1, &handle);
}

void VertexBuffer::bind() const
{
    glew::glBindBuffer(GL_ARRAY_BUFFER, handle);
}
void VertexBuffer::unbind() const
{
    glew::glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace fcm
