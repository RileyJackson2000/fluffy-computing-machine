#include <model/object.hpp>
#include <render/render.hpp>

#include <iostream>
#include <utils/constants.hpp>
#include <utils/glew.hpp>

#include <cmath>

namespace {
static void error_callback(int error, const char *description) {
  (void)error;
  std::cout << "Error: " << description << "\n";
}
} // namespace

namespace fcm {

Window::Window(const std::string &title, uint32_t width, uint32_t height) {
  // Initialise GLFW
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW.");
  }

  // anti aliasing
  glfwWindowHint(GLFW_SAMPLES, 4);
  // version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // some mac os stuff
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // no old opengl
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

  glfwSetErrorCallback(error_callback);

  ptr = GLFWwindow_ptr{
      glfwCreateWindow(width, height, title.c_str(), NULL, NULL)};

  if (ptr.get() == nullptr) {
    glfwTerminate();
    throw std::runtime_error(
        "Failed to open GLFW window. See error logs for more info.");
  }

  glfwMakeContextCurrent(ptr.get());

  // initialize glew
  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLEW.");
  }

  glewExperimental = true; // ? is this needed

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  glfwSetInputMode(ptr.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

Viewer::Viewer()
    : _window{config.windowTitle, config.windowWidth, config.windowHeight},
      _camera{float(config.windowWidth) / float(config.windowHeight)},
      defaultShaderKey{insertShader("default")}, depthShaderKey{insertShader(
                                                     "depth")},
      shadowMaps{}, input{_window.ptr.get()} {
  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

  // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  // add default magenta "not found" texture
  insertTexture(Pixel(0xFF00FFFF));

  // initialize shader uniforms
  auto &&shader = *shaderCache[defaultShaderKey];
  shader.bind();
  shader.setInt("uTexture", DEFAULT);
  shader.setInt("uShadowMap0", SHADOW_0);
  shader.setInt("uShadowMap1", SHADOW_1);
  shader.setInt("uShadowMap2", SHADOW_2);
  shader.setInt("uShadowMap3", SHADOW_3);

  // initialize shadow map on gpu
  shadowMaps.reserve(MAX_NUM_SHADOWS);
  for (size_t i = 0; i < MAX_NUM_SHADOWS; ++i) {
    shadowMaps.emplace_back(config.shadowResolution, config.shadowResolution);
    shadowMaps[i].setTextureUnit(SHADOW_0 + i);
    shadowMaps[i].to_gpu();
  }
}

Viewer::~Viewer() { glfwTerminate(); }

/* RENDER */

void Viewer::renderBeginFrame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  curShadowMap = 0;
}

void Viewer::renderScene(const Scene &scene) {
  renderDirShadows(scene.objects(), scene.dirLights());

  renderPointShadows(scene.objects(), scene.pointLights());
  renderDirLights(scene.dirLights());
  renderPointLights(scene.pointLights());
  renderRigidBodies(scene.objects());
}

void Viewer::renderDirShadows(
    const std::vector<std::unique_ptr<Object>> &objs,
    const std::vector<std::unique_ptr<DirLight>> &lights) {
  glCullFace(GL_FRONT);
  auto &&shader = *shaderCache[defaultShaderKey];
  auto &&depthShader = *shaderCache[depthShaderKey];
  depthShader.bind();

  glViewport(0, 0, config.shadowResolution, config.shadowResolution);

  for (auto &&light : lights) {
    if (curShadowMap >= MAX_NUM_SHADOWS) {
      break;
    }
    shadowMaps[curShadowMap].bind();
    // for eventual point light shadows
    // const float fov = 178.f;
    // glm::mat4 lightP = glm::perspective(glm::radians(fov), 1.f, nearPlane,
    // farPlane);
    glm::mat4 lightP =
        glm::ortho(-30.f, 30.f, -30.f, 30.f, light->nearPlane, light->farPlane);
    glm::vec3 up = {0.f, 1.f, 0.f};
    if (light->dir.x == 0 and light->dir.z == 0) {
      // avoid divide by zero when light is looking straight up or down
      up.y = 0.f;
      up.z = 1.f;
    }

    glm::mat4 lightV = glm::lookAt(light->pos, light->pos + light->dir, up);
    glm::mat4 lightVP = lightP * lightV;
    shader.bind();
    shader.setMat4("uLightVP[" + std::to_string(curShadowMap) + "]", lightVP);

    depthShader.bind();

    glClear(GL_DEPTH_BUFFER_BIT);
    for (auto &&obj : objs) {
      auto MVP = lightVP * obj->getTransform();
      depthShader.setMat4("uMVP", MVP);

      _drawMesh(obj->renderMeshKey);
    }
    ++curShadowMap;
  }
  // reset everything
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_DEPTH_BUFFER_BIT);
  glCullFace(GL_BACK);
  glViewport(0, 0, config.windowWidth, config.windowHeight);
}

void Viewer::renderPointShadows(
    const std::vector<std::unique_ptr<Object>> &,
    const std::vector<std::unique_ptr<PointLight>> &) { /* not implemented */
}

void Viewer::renderRigidBodies(
    const std::vector<std::unique_ptr<Object>> &objs) {
  for (auto &&obj : objs) {
    auto &&shader = *shaderCache[obj->shaderKey];
    shader.bind();

    auto model = obj->getTransform();

    shader.setMat4("uMVP", _vp * model);
    shader.setMat4("uM", model);
    shader.setMat3("uMti", glm::inverse(glm::transpose(model)));

    shader.setVec3("uAmbientColour", obj->ambientColour);
    shader.setVec3("uDiffuseColour", obj->diffuseColour);
    shader.setVec3("uSpecularColour", obj->specularColour);
    shader.setFloat("uShininess", obj->shininess);

    const auto &texture = textureCache[obj->textureKey];
    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    _drawMesh(obj->renderMeshKey);
  }
}

void Viewer::renderDirLights(
    const std::vector<std::unique_ptr<DirLight>> &lights) {
  // n.b. pay attention to this if you ever add a new shader
  auto &&shader = *shaderCache[defaultShaderKey];
  shader.bind();
  for (size_t i = 0; i < lights.size(); ++i) {
    const auto &light = lights[i];
    std::string uDirLights = "uDirLights[" + std::to_string(i) + "].";

    shader.setVec3(uDirLights + "dir", light->dir);

    shader.setVec3(uDirLights + "ambientColour", light->ambientColour);
    shader.setVec3(uDirLights + "diffuseColour", light->diffuseColour);
    shader.setVec3(uDirLights + "specularColour", light->specularColour);
  }
}

void Viewer::renderPointLights(
    const std::vector<std::unique_ptr<PointLight>> &lights) {
  // n.b. pay attention to this if you ever add a new shader
  auto &&shader = *shaderCache[defaultShaderKey];
  shader.bind();
  for (size_t i = 0; i < lights.size(); ++i) {
    const auto &light = lights[i];
    std::string uPointLights = "uPointLights[" + std::to_string(i) + "].";

    shader.setVec3(uPointLights + "pos", light->pos);

    shader.setFloat(uPointLights + "constant", light->constant);
    shader.setFloat(uPointLights + "linear", light->linear);
    shader.setFloat(uPointLights + "quadratic", light->quadratic);

    shader.setVec3(uPointLights + "ambientColour", light->ambientColour);
    shader.setVec3(uPointLights + "diffuseColour", light->diffuseColour);
    shader.setVec3(uPointLights + "specularColour", light->specularColour);
  }
}

void Viewer::renderEndFrame() { glFlush(); }

/* CAMERA */

void Viewer::cameraAddPos(const glm::vec3 &deltaPos) {
  _camera.pos += deltaPos;

  // n.b. pay attention to this if you ever add a new shader
  auto &&shader = *shaderCache[defaultShaderKey];
  shader.bind();
  shader.setVec3("uCamPos", _camera.pos);
  _vp = _camera.projectionMat() * _camera.viewMat();
}

void Viewer::cameraSetPos(const glm::vec3 &newPos) {
  _camera.pos = newPos;

  // n.b. pay attention to this if you ever add a new shader
  auto &&shader = *shaderCache[defaultShaderKey];
  shader.bind();
  shader.setVec3("uCamPos", _camera.pos);
  _vp = _camera.projectionMat() * _camera.viewMat();
}

void Viewer::cameraSetDir(const glm::vec3 &newDir) {
  _camera.dir = newDir;

  _vp = _camera.projectionMat() * _camera.viewMat();
}

const Camera &Viewer::camera() const { return _camera; }

const glm::mat4 &Viewer::cameraVP() const { return _vp; }

RenderMeshKey Viewer::insertMesh(Mesh *mesh) {
  renderMeshCache.emplace_back(std::make_unique<RenderMesh>(mesh));
  return renderMeshCache.size() - 1;
}

TextureKey Viewer::insertTexture(Sprite sprite, TextureUnit tu) {
  textureCache.emplace_back(std::move(sprite));
  textureCache.back().setTextureUnit(tu);
  textureCache.back().to_gpu();
  return textureCache.size() - 1;
}

TextureKey Viewer::insertTexture(Texture t) {
  textureCache.emplace_back(std::move(t));
  return textureCache.size() - 1;
}

ShaderKey Viewer::insertShader(const std::string &shaderName) {
  shaderCache.emplace_back(std::make_unique<Shader>(shaderName));
  return shaderCache.size() - 1;
}

const Window &Viewer::window() const { return _window; }

void Viewer::_drawMesh(const RenderMeshKey &renderMeshKey) const {
  const auto &mesh = *renderMeshCache[renderMeshKey];
  mesh.va.bind();
  mesh.ib.bind();

  glDrawElements(GL_TRIANGLES, mesh.ib.numIndices, GL_UNSIGNED_INT, nullptr);
}

} // namespace fcm
