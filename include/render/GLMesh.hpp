#pragma once

#include <vector>
#include <iostream>

#include <utils/glew.hpp>
#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <render/indexBuffer.hpp>
#include <render/vertexArray.hpp>
#include <render/vertexBuffer.hpp>
#include <render/vertexBufferLayout.hpp>

namespace fcm {

struct GLMeshData {
  MeshData *mesh;
  std::vector<Vertex> mesh_vertices;
  std::vector<uint32_t> mesh_indices;
  VertexBuffer vb;
  IndexBuffer ib;
  VertexArray va;

  explicit GLMeshData(MeshData *meshData)
      : mesh{meshData}, mesh_vertices{mesh->vertices}, mesh_indices{mesh->indices},
        vb{mesh_vertices.data(), sizeof(Vertex) * mesh_vertices.size()},
        ib{mesh_indices.data(), mesh_indices.size()} {
    VertexBufferLayout layout;
    layout.addElem<glew::GLfloat>(3, false); // pos
    layout.addElem<glew::GLfloat>(3, false); // norm

    va.addVertexBuffer(vb, layout);
  }
};

} // namespace fcm
