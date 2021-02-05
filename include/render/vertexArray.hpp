#pragma once

#include <utils/glew.hpp>

#include <render/vertexBuffer.hpp>
#include <render/vertexBufferLayout.hpp>

namespace fcm {
    
struct VertexArray
{
    glew::GLHandle handle;

    VertexArray()
    {
        glew::glGenVertexArrays(1, &handle);
    }
    ~VertexArray()
    {
        glew::glDeleteVertexArrays(1, &handle);
    }

    void addVertexBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
    {
        bind();
        vb.bind();
        for (unsigned int i = 0; i < layout.elems.size(); ++i)
        {
            const auto& elem = layout.elems[i];
            glew::glEnableVertexAttribArray(i);
            // OpenGL is cringe so I had to use reinterpret cast
            glew::glVertexAttribPointer(i, elem.count, elem.type, elem.normalized, layout.numBytes, reinterpret_cast<void *>(elem.offset));
        }
    }

    void bind() const
    {
        glew::glBindVertexArray(handle);
    }

    void unbind() const
    {
        glew::glBindVertexArray(0);
    }
};

} // namespace fcm
