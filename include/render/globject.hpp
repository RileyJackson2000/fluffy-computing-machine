#pragma once

#include <model/object.hpp>
#include <utils/glm.hpp>

namespace fcm {

class GLObject {
  Object *_object;
public:
  GLMeshData glMeshData;

  explicit GLObject(Object *obj): _object{obj}, glMeshData{obj->meshData.get()} {}

  const Object &object() const { return *_object; }

  glm::mat4 getTransform() const;

};

}
