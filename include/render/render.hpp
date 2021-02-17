#pragma once

#include <utils/glfw.hpp>
#include <utils/types.hpp>

#include <model/rayCaster.hpp>
#include <model/scene.hpp>

#include <render/camera.hpp>
#include <render/shader.hpp>

namespace fcm {

// this is really scuffed - We need to load shaders after initializing window -
// TODO fix this this shit causes a bunch of errors - we should figure out how
// to change this
struct Window {
  // TODO resizable window?
  glfw::GLFWwindow_ptr ptr;
  Window(uint32_t width, uint32_t height);

  int getKey(int key) { return glfw::glfwGetKey(ptr.get(), key); }
  int getMouseButton(int key) {
    return glfw::glfwGetMouseButton(ptr.get(), key);
  }
  glm::vec2 getCursorPos() {
    double x, y;
    glfw::glfwGetCursorPos(ptr.get(), &x, &y);
    return glm::vec2{x, y};
  }
  void setShouldClose(bool b) { glfw::glfwSetWindowShouldClose(ptr.get(), b); }
  bool shouldClose() { return glfw::glfwWindowShouldClose(ptr.get()); }
};

struct Viewer {
  Window window;
  Shader shader; // shaders should be part of materials. We should also support
                 // more than one shader
  RenderObjectCache *renderObjectCache;
  RayCaster *rayCaster;
  Camera *cam;

  Viewer(RenderObjectCache *, RayCaster *, Camera *);
  ~Viewer();

  void render(Scene *);
  void draw(RenderObjectKey renderObjectKey);

  // controller - TODO: move this to separete class
  float movementSpeed = 0.1;
  double lastFrameTime;

  glm::vec2 lastMousePos{WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f};
  float yaw = -90.f;
  float pitch = 0;
  float mouseSensitivity = 0.15;
  bool rbuttonDown = false;

  void updateCameraPos(double dt);
  void updateCameraDir();
  void selectObject(Scene &scene);
  bool closeWindow();

  void pollEvents() { glfw::glfwPollEvents(); }
  double getTime() { return glfw::glfwGetTime(); }
};

} // namespace fcm
