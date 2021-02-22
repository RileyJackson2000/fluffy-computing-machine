#include <model/object.hpp>
#include <render/render.hpp>

#include <iostream>
#include <render/renderObject.hpp>
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
  if (!glfw::glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW.");
  }

  // anti aliasing
  glfw::glfwWindowHint(GLFW_SAMPLES, 4);
  // version
  glfw::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfw::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // some mac os stuff
  glfw::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // no old opengl
  glfw::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfw::glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

  glfw::glfwSetErrorCallback(error_callback);

  ptr = glfw::GLFWwindow_ptr{
      glfw::glfwCreateWindow(width, height, title.c_str(), NULL, NULL)};

  if (ptr.get() == nullptr) {
    glfw::glfwTerminate();
    throw std::runtime_error(
        "Failed to open GLFW window. See error logs for more info.");
  }

  glfw::glfwMakeContextCurrent(ptr.get());

  // initialize glew
  if (glew::glewInit() != GLEW_OK) {
    glfw::glfwTerminate();
    throw std::runtime_error("Failed to initialize GLEW.");
  }

  glew::glewExperimental = true; // ? is this needed

  std::cout << "OpenGL Version: " << glew::glGetString(GL_VERSION) << std::endl;

  glfw::glfwSetInputMode(ptr.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

Viewer::Viewer(RenderObjectCache *renderObjectCache)
    : window{Config::windowTitle, Config::windowWidth, Config::windowHeight},
      shader{std::string{"default"}}, renderObjectCache{renderObjectCache} {
  // Dark blue background
  glew::glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

  // glew::glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  glew::glEnable(GL_CULL_FACE);
  glew::glEnable(GL_DEPTH_TEST);

  lastFrameTime = getTime();
}

Viewer::~Viewer() { glfw::glfwTerminate(); }

void Viewer::render(RenderScene &scene) {
  // controller
  pollEvents();
  double currentTime = getTime();
  double dt = currentTime - lastFrameTime; // TODO should be passed in?
  lastFrameTime = currentTime;
  updateCameraPos(dt, scene);
  updateCameraDir(scene);
  selectObject(scene);

  glew::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _bindLights(scene);

  shader.setVec3("uCamPos", scene.camera.pos);

  glm::mat4 vp = scene.camera.projectionMat() * scene.camera.viewMat();

  for (auto &&obj : scene.objects()) {
    shader.setMat4("uMVP", vp * obj->getTransform());
    shader.setMat4("uM", obj->getTransform());
    shader.setMat3("uMti", glm::inverse(glm::transpose(obj->getTransform())));

    shader.setVec3("uAmbientColour", obj->ambientColour);
    shader.setVec3("uDiffuseColour", obj->diffuseColour);
    shader.setVec3("uSpecularColour", obj->specularColour);
    shader.setFloat("uShininess", obj->shininess);

    draw(obj->renderObjectKey);
  }

  glew::glFlush();
}

void Viewer::draw(RenderObjectKey renderObjectKey) {
  const auto &model = *((*renderObjectCache)[renderObjectKey]);
  shader.bind();
  model.va.bind();
  model.ib.bind();

  glew::glDrawElements(GL_TRIANGLES, model.ib.numIndices, GL_UNSIGNED_INT,
                       nullptr);
}

void Viewer::updateCameraPos(double dt, RenderScene &scene) {
  float speed;
  if (window.getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    speed = movementSpeed * 0.33;
  else
    speed = movementSpeed;

  float multiplier = speed * (dt * 1000.f);
  if (window.getKey(GLFW_KEY_W) == GLFW_PRESS)
    scene.camera.pos += multiplier * scene.camera.dir;
  if (window.getKey(GLFW_KEY_S) == GLFW_PRESS)
    scene.camera.pos -= multiplier * scene.camera.dir;
  if (window.getKey(GLFW_KEY_A) == GLFW_PRESS)
    scene.camera.pos -=
        multiplier *
        glm::normalize(glm::cross(scene.camera.dir, scene.camera.up));
  if (window.getKey(GLFW_KEY_D) == GLFW_PRESS)
    scene.camera.pos +=
        multiplier *
        glm::normalize(glm::cross(scene.camera.dir, scene.camera.up));
}

void Viewer::updateCameraDir(RenderScene &scene) {
  int state = window.getMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
  if (state == GLFW_PRESS) {
    glm::vec2 pos = window.getCursorPos();
    if (!rbuttonDown) {
      lastMousePos = pos;
    }
    rbuttonDown = true;
    float xoffset = pos.x - lastMousePos.x;
    float yoffset = lastMousePos.y - pos.y;
    lastMousePos = pos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    glm::clamp(pitch, -89.f, 89.f);
    scene.camera.dir.x =
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    scene.camera.dir.y = glm::sin(glm::radians(pitch));
    scene.camera.dir.z =
        glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    scene.camera.dir = glm::normalize(scene.camera.dir);
  } else if (state == GLFW_RELEASE) {
    rbuttonDown = false;
  }
}

void Viewer::selectObject(RenderScene &scene) {
  RayCastResult result;
  if (window.getMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    glm::vec2 pos = window.getCursorPos();
    glm::mat4 invCam =
        glm::inverse(scene.camera.projectionMat() * scene.camera.viewMat());
    float hw = Config::windowWidth / 2.f;
    float hh = Config::windowHeight / 2.f;
    glm::vec4 near =
        glm::vec4((pos.x - hw) / hw, -1 * (pos.y - hh) / hh, -1, 1.0);
    glm::vec4 far =
        glm::vec4((pos.x - hw) / hw, -1 * (pos.y - hh) / hh, 1, 1.0);
    glm::vec4 nearResult = invCam * near;
    glm::vec4 farResult = invCam * far;
    nearResult /= nearResult.w;
    farResult /= farResult.w;
    glm::vec3 dir = glm::normalize(glm::vec3(farResult - nearResult));

    result = scene.rayCaster.castRay(scene.camera.pos, dir, scene.objects());
    if (result.hit) {
      result.object->ambientColour = glm::vec3{0.f, 0.1f, 0.f};
      result.object->diffuseColour = glm::vec3{0.f, 0.5f, 0.f};
    }
  }
}

// todo - have a controller for pressed keys
bool Viewer::closeWindow() {
  if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    window.setShouldClose(true);
  return window.shouldClose();
}

void Viewer::_bindLights(const RenderScene &scene) {
  // dir lights
  for (size_t i = 0; i < scene.dirLights().size(); ++i) {
    const auto &light = scene.dirLights()[i];
    std::string uDirLights = "uDirLights[" + std::to_string(i) + "].";

    shader.setVec3(uDirLights + "dir", light->dir);

    shader.setVec3(uDirLights + "ambientColour", light->ambientColour);
    shader.setVec3(uDirLights + "diffuseColour", light->diffuseColour);
    shader.setVec3(uDirLights + "specularColour", light->specularColour);
  }
  shader.setInt("numDirLights", scene.dirLights().size());

  // point lights
  for (size_t i = 0; i < scene.pointLights().size(); ++i) {
    const auto &light = scene.pointLights()[i];
    std::string uPointLights = "uPointLights[" + std::to_string(i) + "].";

    shader.setVec3(uPointLights + "pos", light->pos);

    shader.setFloat(uPointLights + "constant", light->constant);
    shader.setFloat(uPointLights + "linear", light->linear);
    shader.setFloat(uPointLights + "quadratic", light->quadratic);

    shader.setVec3(uPointLights + "ambientColour", light->ambientColour);
    shader.setVec3(uPointLights + "diffuseColour", light->diffuseColour);
    shader.setVec3(uPointLights + "specularColour", light->specularColour);
  }
  shader.setInt("numPointLights", scene.pointLights().size());
}

} // namespace fcm
