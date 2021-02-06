#pragma once

#include <utils/glew.hpp>

namespace fcm {
    
struct VertexBuffer
{
    glew::GLHandle handle;

    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
};

} // namespace fcm
