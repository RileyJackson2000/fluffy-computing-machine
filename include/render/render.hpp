#pragma once

#include <config.hpp>

#include <utils/glfw.hpp>
#include <utils/types.hpp>

#include <model/camera.hpp>
#include <model/rayCaster.hpp>
#include <model/renderScene.hpp>

#include <render/shader.hpp>

namespace fcm {

// this is really scuffed - We need to load shaders after initializing window -
// TODO fix this this shit causes a bunch of errors - we should figure out how
// to change this
struct Window {
  // TODO resizable window?
  GLFWwindow_ptr ptr;
  Window(const std::string &title, uint32_t width, uint32_t height);

  int getKey(int key) { return glfwGetKey(ptr.get(), key); }
  int getMouseButton(int key) { return glfwGetMouseButton(ptr.get(), key); }
  glm::vec2 getCursorPos() {
    double x, y;
    glfwGetCursorPos(ptr.get(), &x, &y);
    return glm::vec2{x, y};
  }
  void setShouldClose(bool b) { glfwSetWindowShouldClose(ptr.get(), b); }
  bool shouldClose() { return glfwWindowShouldClose(ptr.get()); }
};

struct Viewer {
  Config config;
  Window window;
  Shader shader; // shaders should be part of materials. We should also support
                 // more than one shader
  RenderObjectCache *renderObjectCache;

  Viewer(Config, RenderObjectCache *);
  ~Viewer();

  void render(RenderScene &);
  void draw(RenderObjectKey renderObjectKey);

  // controller - TODO: move this to separete class
  float movementSpeed = 0.1;
  double lastFrameTime;

  glm::vec2 lastMousePos{config.windowWidth / 2.f, config.windowHeight / 2.f};
  float yaw = -90.f;
  float pitch = 0;
  float mouseSensitivity = 0.15;
  bool rbuttonDown = false;

  void updateCameraPos(double dt, RenderScene &);
  void updateCameraDir(RenderScene &);
  void selectObject(RenderScene &);
  bool closeWindow();

  void pollEvents() { glfwPollEvents(); }
  double getTime() { return glfwGetTime(); }

private:
  // copy lights to the gpu
  void _bindLights(const RenderScene &scene);
};

} // namespace fcm
