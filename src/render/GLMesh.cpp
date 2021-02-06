#include <vector>

#include <utils/glew.hpp>

#include <model/mesh.hpp>
#include <render/GLMesh.hpp>
#include <render/indexBuffer.hpp>
#include <render/vertexArray.hpp>
#include <render/vertexBuffer.hpp>

namespace fcm {

GLMeshData::GLMeshData(const MeshData &meshData)
    : mesh{meshData}, vb{mesh.vertices.data(),
                         sizeof(Vertex) * mesh.vertices.size()},
      ib{mesh.indices.data(), mesh.indices.size()} {
  VertexBufferLayout layout;
  layout.addElem<glew::GLfloat>(3); // pos
  layout.addElem<glew::GLfloat>(3); // norm

  va.addVertexBuffer(vb, layout);
}

} // namespace fcm
