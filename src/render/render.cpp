#include <render/render.hpp>

#include <utils/glew.hpp>
#include <iostream>

namespace fcm {

static void error_callback(int error, const char *description)
{
  (void) error;
  std::cout << "Error: " << description << "\n";
}

glfw::GLFWwindow_ptr init_renderer(void)
{
  // Initialise GLFW
  if(!glfw::glfwInit())
  {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return glfw::GLFWwindow_ptr{};
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

  glfw::glfwSetErrorCallback(error_callback);

  // TODO jyfliu: don't hardcode constants
  glfw::GLFWwindow_ptr window = glfw::GLFWwindow_ptr{glfw::glfwCreateWindow(1024, 768, "Title goes here", NULL, NULL)};
  if (window.get() == nullptr) {
    std::cout << "Failed to open GLFW window. See error logs for more info.\n";
    glfw::glfwTerminate();
    return glfw::GLFWwindow_ptr{};
  }
  glfw::glfwMakeContextCurrent(window.get());

  // initialize glew
  if (glew::glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize GLEW\n";
    glfw::glfwTerminate();
    return glfw::GLFWwindow_ptr{};
  }

  glew::glewExperimental=true;

  // Ensure we can capture the escape key being pressed below
  glfw::glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glew::glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

  return window;
}

void render(Scene &scene, glfw::GLFWwindow *window) {
  (void) scene;
  glew::glClear(GL_COLOR_BUFFER_BIT);

  for (auto &&obj : scene.objects()) {
    std::cout << obj->name << " (" << obj->position.x << ", " << obj->position.y << ", "<< obj->position.z << "), ";
  }
  std::cout << "\n";
  
  glfw::glfwSwapBuffers(window);
  glfw::glfwPollEvents();
}

void destroy_renderer() {
  glfw::glfwTerminate();
}

} // namespace fcm
