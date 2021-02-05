#pragma once

#include <model/object.hpp>
#include <utils/glm.hpp>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>

#include <render/GLMesh.hpp>
#include <model/mesh.hpp>


namespace fcm {

static MeshData sphereMeshData = genSphereMesh(1.f, 50, 50);
static GLMeshData *GLSphereMeshData = nullptr;

class Scene {
  // std string for testing purposes
  std::string _name;
  std::vector<std::unique_ptr<Object>> _objects;

  // TODO make static mesh cache

public:
  Scene(std::string name): _name{std::move(name)}
  {
    GLSphereMeshData = new GLMeshData{sphereMeshData};
  }
  const std::string &name() const {return _name;}

  void insert(std::unique_ptr<Object> obj, glm::vec3 scale)
  {
    // this stuff should happen elsewhere
    // since we need to be able to cache meshes thoughout the sim, im putting it here for now - Ayon
    obj->mesh.meshData = GLSphereMeshData;
    glm::mat4 xform{1.0f};
    xform = glm::scale(xform, scale);
    obj->mesh.transform = xform;

    _objects.emplace_back(std::move(obj));
  }

  const std::vector<std::unique_ptr<Object>> &objects() const { return _objects; }
};

}
