#pragma once

#include <utils/glfw.hpp>

namespace fcm {

/* OpenGL-specific input manager */
class Input {
  GLFWwindow *_window;

public:
  Input(GLFWwindow *);

  void pollInput() const;

  /* friendly helper functions */
  bool keyPressed(int key) const;
  bool keyReleased(int key) const;
  bool mousePressed(int key) const;
  bool mouseReleased(int key) const;
  glm::vec2 cursorPos() const;

  /* get raw key inputs (not recommended) */
  int getKey(int key) const;
  int getMouseButton(int key) const;
};

} // namespace fcm
