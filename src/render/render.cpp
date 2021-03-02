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
    : _camera{float(config.windowWidth) / float(config.windowHeight)},
      window{config.windowTitle, config.windowWidth, config.windowHeight},
      input{window.ptr.get()}, shader{std::string{"default"}} {
  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

  // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
}

Viewer::~Viewer() { glfwTerminate(); }

/* RENDER */

void Viewer::renderBeginFrame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Viewer::renderRigidBodies(
    const std::vector<std::unique_ptr<Object>> &objs) {
  for (auto &&obj : objs) {
    shader.setMat4("uMVP", _vp * obj->getTransform());
    shader.setMat4("uM", obj->getTransform());
    shader.setMat3("uMti", glm::inverse(glm::transpose(obj->getTransform())));

    shader.setVec3("uAmbientColour", obj->ambientColour);
    shader.setVec3("uDiffuseColour", obj->diffuseColour);
    shader.setVec3("uSpecularColour", obj->specularColour);
    shader.setFloat("uShininess", obj->shininess);

    _drawMesh(obj->renderMeshKey);
  }
}

void Viewer::renderDirLights(
    const std::vector<std::unique_ptr<DirLight>> &lights) {
  for (size_t i = 0; i < lights.size(); ++i) {
    const auto &light = lights[i];
    std::string uDirLights = "uDirLights[" + std::to_string(i) + "].";

    shader.setVec3(uDirLights + "dir", light->dir);

    shader.setVec3(uDirLights + "ambientColour", light->ambientColour);
    shader.setVec3(uDirLights + "diffuseColour", light->diffuseColour);
    shader.setVec3(uDirLights + "specularColour", light->specularColour);
  }

  shader.setInt("numDirLights", lights.size());
}

void Viewer::renderPointLights(
    const std::vector<std::unique_ptr<PointLight>> &lights) {
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

  shader.setInt("numPointLights", lights.size());
}

void Viewer::renderEndFrame() { glFlush(); }

/* CAMERA */

void Viewer::cameraAddPos(const glm::vec3 &deltaPos) {
  _camera.pos += deltaPos;

  shader.setVec3("uCamPos", _camera.pos);
  _vp = _camera.projectionMat() * _camera.viewMat();
}

void Viewer::cameraSetPos(const glm::vec3 &newPos) {
  _camera.pos = newPos;

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

void Viewer::_drawMesh(const RenderMeshKey &renderMeshKey) const {
  const auto &mesh = *renderMeshCache[renderMeshKey];
  shader.bind();
  mesh.va.bind();
  mesh.ib.bind();

  glDrawElements(GL_TRIANGLES, mesh.ib.numIndices, GL_UNSIGNED_INT, nullptr);
}

} // namespace fcm
