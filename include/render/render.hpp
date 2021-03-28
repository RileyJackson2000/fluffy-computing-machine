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
#include <render/shadowMap.hpp>
#include <render/texture.hpp>

namespace fcm {

using RenderMeshCache = std::vector<std::unique_ptr<RenderMesh>>;
using TextureCache = std::vector<Texture>;

// due to opengl limitations, we can only have 16 textures active
// for a single fragment, and they must be mapped manually
// do that mapping here
enum TextureUnit {
  // use this for everyday, opaque textures
  DEFAULT = 0,
  // reserved for shadows (this limits the max num of lights with shadows)
  SHADOW_0 = 1,
  SHADOW_1 = 2,
  SHADOW_2 = 3,
  SHADOW_3 = 4,
  // add more if necessary
};

// make sure to update the fragment shader if you change this value
inline constexpr int MAX_NUM_SHADOWS = 4;

struct Window {
  // TODO resizable window?
  GLFWwindow_ptr ptr;
  Window(const std::string &title, uint32_t width, uint32_t height);

  void setShouldClose(bool b) const { glfwSetWindowShouldClose(ptr.get(), b); }
  bool shouldClose() const { return glfwWindowShouldClose(ptr.get()); }
};

/* OpenGL-specific implementation of a viewer */
class Viewer {
  Window _window;
  glm::mat4 _vp;
  Camera _camera;
  RenderMeshCache renderMeshCache;
  TextureCache textureCache;
  ShaderCache shaderCache;

  // shaders used by viewer
  ShaderKey defaultShaderKey;
  ShaderKey depthShaderKey;

  // state used by shadow renderer
  std::vector<ShadowMap> shadowMaps;
  size_t curShadowMap;

public:
  Input input;

  Viewer();
  ~Viewer();

  /* render */
  // always call this to begin a render cycle
  void renderBeginFrame();
  // call this if you want to render everything
  void renderScene(const Scene &);
  // otherwise call these to render specific entities (order is very important)

  // render shadows. Note that there are a limited number k of shadow map
  // texture units (defined above). That means, in a certain frame, only
  // the first k lights will have their shadows drawn, and the remaining
  // lights won't have shadows
  // (if you want to choose which lights have their shadows drawn...
  // then its not implemented yet)
  void renderDirShadows(const std::vector<std::unique_ptr<Object>> &,
                        const std::vector<std::unique_ptr<DirLight>> &);
  void renderPointShadows(const std::vector<std::unique_ptr<Object>> &,
                          const std::vector<std::unique_ptr<PointLight>> &);

  // render lights. A bit of a misnomer, these functions don't render
  // anything until renderRigidBodies is called, they just copy the light
  // information to the gpu
  void renderDirLights(const std::vector<std::unique_ptr<DirLight>> &);
  void renderPointLights(const std::vector<std::unique_ptr<PointLight>> &);

  // render meshes.
  void renderRigidBodies(const std::vector<std::unique_ptr<Object>> &);
  // always call this to end the render cycle
  void renderEndFrame();

  /* camera */
  // these functions communicate with the gpu, so don't call them too frequently
  void cameraAddPos(const glm::vec3 &deltaPos);
  void cameraSetPos(const glm::vec3 &newPos);
  void cameraSetDir(const glm::vec3 &newDir);

  const Camera &camera() const;
  const glm::mat4 &cameraVP() const;

  /* render assets */
  RenderMeshKey insertMesh(Mesh *);
  TextureKey insertTexture(Sprite, TextureUnit tu = DEFAULT);
  TextureKey insertTexture(Texture);
  ShaderKey insertShader(const std::string &shaderName);

  /* getters */
  const Window &window() const;

private:
  void _drawMesh(const RenderMeshKey &) const;
};

} // namespace fcm
