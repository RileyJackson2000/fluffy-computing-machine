#pragma once

#include <vector>

#include <utils/glew.hpp>
#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <render/indexBuffer.hpp>
#include <render/vertexArray.hpp>
#include <render/vertexBuffer.hpp>
#include <render/vertexBufferLayout.hpp>

namespace fcm {

struct GLMeshData {
  MeshData mesh;
  VertexBuffer vb;
  IndexBuffer ib;
  VertexArray va;

  GLMeshData(const MeshData &meshData)
      : mesh{meshData}, vb{mesh.vertices.data(),
                           sizeof(Vertex) * mesh.vertices.size()},
        ib{mesh.indices.data(), mesh.indices.size()} {
    VertexBufferLayout layout;
    layout.addElem<glew::GLfloat>(3, false); // pos
    layout.addElem<glew::GLfloat>(3, false); // norm

    va.addVertexBuffer(vb, layout);
  }
};

} // namespace fcm
