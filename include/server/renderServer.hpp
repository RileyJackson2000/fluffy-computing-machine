#pragma once

#include <utils/config.hpp>

#include <model/camera.hpp>
#include <model/rayCaster.hpp>
#include <model/scene.hpp>

// we can make this import depend on an include guard
// this way, we can select different renderers (OpenGL, DX11, vulkan, etc)
// in the very far future
#include <render/input.hpp>
#include <render/render.hpp>

namespace fcm {

class RenderServer {
  Viewer _viewer;
  Input _input;

  // TODO clean up
  float movementSpeed = 0.1;
  double lastFrameTime;
  glm::vec2 lastMousePos{config.windowWidth / 2.f, config.windowHeight / 2.f};
  float yaw = -90.f;
  float pitch = 0;
  float mouseSensitivity = 0.15;
  bool rbuttonDown = false;

public:
  RenderServer();

  // render the current scene
  void render(Scene &);

  // getInput returns the input (keys pressed) since it was last called (TODO)
  void getInput();
  bool shouldClose();

  /* render mesh */
  RenderMeshKey insertMesh(Mesh *);

  // TODO remove and replace with std chrono
  double getTime() const { return glfwGetTime(); }

private:
  // updates cached input to be returned by `getInput`
  void pollInput();

  // update camera depending on input
  void updateCameraPos(double dt);
  void updateCameraDir(double dt);

  // spaghetti, this method needs a proper home (TODO)
  void selectObject(Scene &);
};

} // namespace fcm
