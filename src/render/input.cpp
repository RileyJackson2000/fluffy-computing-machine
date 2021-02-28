#include <render/input.hpp>

namespace fcm {

Input::Input(GLFWwindow *window) : _window{window} {}

void Input::pollInput() const { glfwPollEvents(); }

bool Input::keyPressed(int key) const { return getKey(key) == GLFW_PRESS; }

bool Input::mousePressed(int key) const {
  return getMouseButton(key) == GLFW_PRESS;
}

bool Input::keyReleased(int key) const { return getKey(key) == GLFW_RELEASE; }

bool Input::mouseReleased(int key) const {
  return getMouseButton(key) == GLFW_RELEASE;
}

glm::vec2 Input::cursorPos() const {
  double x, y;
  glfwGetCursorPos(_window, &x, &y);
  return glm::vec2{x, y};
}

int Input::getKey(int key) const { return glfwGetKey(_window, key); }

int Input::getMouseButton(int key) const {
  return glfwGetMouseButton(_window, key);
}

} // namespace fcm
