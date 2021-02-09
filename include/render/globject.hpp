#pragma once

#include <model/object.hpp>
#include <utils/glm.hpp>

namespace fcm {

class GLObject {
  Object *_object;
public:
  GLMeshData glMeshData;

  GLObject(const GLObject &x) : glMeshData{genSphereMesh(0.0001,10,10,true).get()}
  {
    std::cout << "copy ctor GLObject" << std::endl;
  }

  GLObject(const GLObject &&x) : glMeshData{genSphereMesh(0.0001,10,10,true).get()}
  {
    std::cout << "move ctor GLObject" << std::endl;
  }
  
  explicit GLObject(Object *obj): _object{obj}, glMeshData{obj->meshData.get()}
  {
    std::cout << "created GLObject" << std::endl;
  }

  const Object &object() const { return *_object; }

  glm::mat4 getTransform() const;

};

}
