#pragma once

#include <iostream>
#include <vector>

#include <utils/glew.hpp>
#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <render/indexBuffer.hpp>
#include <render/vertexArray.hpp>
#include <render/vertexBuffer.hpp>
#include <render/vertexBufferLayout.hpp>

namespace fcm {

struct RenderMesh {
  VertexBuffer vb;
  IndexBuffer ib;
  VertexArray va;

  explicit RenderMesh(Mesh *mesh)
      : vb{mesh->vertices.data(), sizeof(Vertex) * mesh->vertices.size()},
        ib{mesh->indices.data(), mesh->indices.size()} {
    VertexBufferLayout layout;
    layout.addElem<GLfloat>(3, false); // pos
    layout.addElem<GLfloat>(3, false); // norm
    layout.addElem<GLfloat>(2, false); // uv

    va.addVertexBuffer(vb, layout);
  }
  // non-copyable, non-movable, as well as vb, ib, va
  // the reasoning is, vb, ib, va own resources on the gpu through the glhandle,
  // and this resource is released in the destructor
  // maintaining this resource through copies and moves is extra annoying and
  // not worth the hassle. Instead, always maintain pointers to RenderMesh
  RenderMesh(const RenderMesh &) = delete;
  RenderMesh(RenderMesh &&) = delete;
  RenderMesh &operator=(RenderMesh other) = delete;
  ~RenderMesh() {}
};

} // namespace fcm
