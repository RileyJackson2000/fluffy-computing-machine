#pragma once

#include <model/object.hpp>
#include <utils/glm.hpp>

namespace fcm {

class GLObject {
  Object *_object;
  std::unique_ptr<GLMeshData> _glMeshData;

public:
  explicit GLObject(Object *obj)
      : _object{obj}, _glMeshData{
                          std::make_unique<GLMeshData>(obj->meshData.get())} {}
  // no copy constructor since opengl mesh is not copyable
  // if you want to copy globj, explicitly create a new GLObject(globj.object())
  GLObject(const GLObject &) = delete;
  GLObject(GLObject &&) = default;
  ~GLObject() {}

  const GLMeshData &glMeshData() const { return *_glMeshData; }

  const Object &object() const { return *_object; }

  glm::mat4 getTransform() const;
};

} // namespace fcm
