#pragma once

#include <vector>

#include <utils/glew.hpp>


namespace fcm {

template <typename T>
constexpr glew::GLenum getTypeEnum()
{
    if constexpr (std::is_same<T, glew::GLuint>::value || std::is_same<T, unsigned int>::value)
        return GL_UNSIGNED_INT;
    if constexpr (std::is_same<T, glew::GLfloat>::value || std::is_same<T, float>::value)
        return GL_FLOAT;
    // add types here as necessary

    return 0;
    
    // idk why this doesnt work 
    // static_assert(false, "Unrecognized type in fcm::vertexBufferLayout::getTypeEnum");
}

struct VertexBufferLayoutElem
{
    glew::GLenum type;
    unsigned int count;
    glew::GLint normalized;

    unsigned int offset;
};

struct VertexBufferLayout
{
    std::vector<VertexBufferLayoutElem> elems;
    unsigned int numBytes; // stride

    VertexBufferLayout() : numBytes{0} {}
    ~VertexBufferLayout() {}

    template <typename T>
    void addElem(unsigned int count, bool normalized = false)
    {
        // struct packing issues possible?
        elems.push_back({ getTypeEnum<T>(), count, normalized ? GL_TRUE : GL_FALSE, numBytes });
        numBytes += count * sizeof(T);
    }
};

} // namespace fcm
