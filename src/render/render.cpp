#include <model/object.hpp>
#include <render/render.hpp>
#include <render/render2d.hpp>
#include <render/shaders.hpp>

#include <iostream>
#include <utils/constants.hpp>
#include <utils/glew.hpp>

#include <cmath>

namespace fcm {

static void error_callback(int error, const char *description) {
  (void)error;
  std::cout << "Error: " << description << "\n";
}

// statics
glew::GLuint VertexArrayID;
glew::GLuint programID;

glfw::GLFWwindow_ptr init_renderer(void) {
  // Initialise GLFW
  if (!glfw::glfwInit()) {
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

  glfw::GLFWwindow_ptr window = glfw::GLFWwindow_ptr{glfw::glfwCreateWindow(
      WINDOW_WIDTH, WINDOW_HEIGHT, "Title goes here", NULL, NULL)};
  if (window.get() == nullptr) {
    std::cerr << "Failed to open GLFW window. See error logs for more info.\n";
    glfw::glfwTerminate();
    return glfw::GLFWwindow_ptr{};
  }
  glfw::glfwMakeContextCurrent(window.get());

  // initialize glew
  if (glew::glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW\n";
    glfw::glfwTerminate();
    return glfw::GLFWwindow_ptr{};
  }

  glew::glewExperimental = true;

  // Ensure we can capture the escape key being pressed below
  glfw::glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glew::glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

  glew::glGenVertexArrays(1, &VertexArrayID);
  glew::glBindVertexArray(VertexArrayID);

  programID = load_shaders();

  if (!programID) {
    std::cerr << "Failed to initialize shaders\n";
    glfw::glfwTerminate();
    return glfw::GLFWwindow_ptr{};
  }

  return window;
}

void render(Scene &scene, glfw::GLFWwindow *window) {
  glew::glClear(GL_COLOR_BUFFER_BIT);
  glew::glUseProgram(programID);

  for (auto &&obj : scene.objects()) {
    // not good
    auto *sphere = static_cast<Sphere *>(obj.get());
    drawCircle(sphere->position.x, sphere->position.y, sphere->radius);
  }

  glfw::glfwSwapBuffers(window);
  glfw::glfwPollEvents();
}

void destroy_renderer(glfw::GLFWwindow_ptr window) {
  glfw::glfwDestroyWindow(window.release());
  glew::glDeleteVertexArrays(1, &VertexArrayID);
  glfw::glfwTerminate();
}

} // namespace fcm
