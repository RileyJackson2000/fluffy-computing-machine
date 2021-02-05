#pragma once

#include <vector>

#include <utils/glew.hpp>
#include <utils/glm.hpp>

#include <render/vertexBuffer.hpp>
#include <render/indexBuffer.hpp>
#include <render/vertexArray.hpp>
#include <model/mesh.hpp>

namespace fcm {

struct GLMeshData
{
    MeshData mesh;
    VertexBuffer vb;
    IndexBuffer ib;
    VertexArray va;

    GLMeshData(const MeshData &meshData)
    :   mesh{meshData},
        vb{mesh.vertices.data(), sizeof(glm::vec3) * mesh.vertices.size()},
        ib{mesh.indices.data(), mesh.indices.size()}
    {
        VertexBufferLayout layout;
        layout.addElem<glew::GLfloat>(3); // pos
        layout.addElem<glew::GLfloat>(3); // norm

        va.addVertexBuffer(vb, layout);
    }
};

struct GLMesh
{
    GLMeshData *meshData;
    glm::mat4 transform;
};

} // namespace fcm
