#pragma once

#include <utils/config.hpp>
#include <utils/glfw.hpp>
#include <utils/types.hpp>

#include <model/camera.hpp>
#include <model/light.hpp>
#include <model/rayCaster.hpp>
#include <model/scene.hpp>

#include <render/input.hpp>
#include <render/renderMesh.hpp>
#include <render/shader.hpp>
#include <render/texture.hpp>

namespace fcm {

using RenderMeshCache = std::vector<std::unique_ptr<RenderMesh>>;
using TextureCache = std::vector<Texture>;

// this is really scuffed - We need to load shaders after initializing window -
// TODO fix this this shit causes a bunch of errors - we should figure out how
// to change this
struct Window {
  // TODO resizable window?
  GLFWwindow_ptr ptr;
  Window(const std::string &title, uint32_t width, uint32_t height);

  void setShouldClose(bool b) const { glfwSetWindowShouldClose(ptr.get(), b); }
  bool shouldClose() const { return glfwWindowShouldClose(ptr.get()); }
};

/* OpenGL-specific implementation of a viewer */
class Viewer {
  glm::mat4 _vp;
  Camera _camera;
  RenderMeshCache renderMeshCache;
  TextureCache textureCache;

public:
  Window window;
  Input input;
  Shader shader; // shaders should be part of materials. We should also support
                 // more than one shader

  Viewer();
  ~Viewer();

  /* render */
  void renderBeginFrame();
  void renderRigidBodies(const std::vector<std::unique_ptr<Object>> &);
  void renderDirLights(const std::vector<std::unique_ptr<DirLight>> &);
  void renderPointLights(const std::vector<std::unique_ptr<PointLight>> &);
  void renderEndFrame();

  /* camera */
  // these functions communicate with the gpu, so don't call them too frequently
  void cameraAddPos(const glm::vec3 &deltaPos);
  void cameraSetPos(const glm::vec3 &newPos);
  void cameraSetDir(const glm::vec3 &newDir);

  const Camera &camera() const;
  const glm::mat4 &cameraVP() const;

  /* render assets */
  RenderMeshKey createRenderMesh(Mesh *);
  TextureKey createTexture(Image);

private:
  void _drawMesh(const RenderMeshKey &, const TextureKey &) const;
};

} // namespace fcm
