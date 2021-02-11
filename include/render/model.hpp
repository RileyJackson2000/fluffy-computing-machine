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

struct Model {
  MeshData *mesh;
  VertexBuffer vb;
  IndexBuffer ib;
  VertexArray va;

  explicit Model(MeshData *meshData)
      : mesh{meshData}, vb{meshData->vertices.data(),
                           sizeof(Vertex) * meshData->vertices.size()},
        ib{meshData->indices.data(), meshData->indices.size()} {
    VertexBufferLayout layout;
    layout.addElem<glew::GLfloat>(3, false); // pos
    layout.addElem<glew::GLfloat>(3, false); // norm

    va.addVertexBuffer(vb, layout);
  }
  // non-copyable, non-movable, as well as vb, ib, va
  // the reasoning is, vb, ib, va own resources on the gpu through the glhandle,
  // and this resource is released in the destructor
  // maintaining this resource through copies and moves is extra annoying and
  // not worth the hassle. Instead, always maintain pointers to Model
  Model(const Model &) = delete;
  Model(Model &&) = delete;
  Model &operator=(Model other) = delete;
  ~Model() {}
};

} // namespace fcm
