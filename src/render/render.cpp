#include <model/object.hpp>
#include <render/render.hpp>
#include <render/render2d.hpp>
#include <render/shaders.hpp>

#include <utils/constants.hpp>
#include <utils/glew.hpp>
#include <iostream>

namespace {
    static void error_callback(int error, const char *description)
    {
        (void) error;
        std::cout << "Error: " << description << "\n";
    }
} // namespace

namespace fcm {
glew::GLuint programID;
glew::GLuint VertexArrayID;

Window::Window(unsigned int width, unsigned int height) {
    // Initialise GLFW
    if(!glfw::glfwInit())
    {
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

    glfw::glfwSetErrorCallback(error_callback);

    ptr = glfw::GLFWwindow_ptr{
    glfw::glfwCreateWindow(width, height, "Title goes here", NULL, NULL)
    };

    if (ptr.get() == nullptr) {
    glfw::glfwTerminate();
    throw std::runtime_error("Failed to open GLFW window. See error logs for more info.");
    }

    glfw::glfwMakeContextCurrent(ptr.get());

    // initialize glew
    if (glew::glewInit() != GLEW_OK) {
    glfw::glfwTerminate();
    throw std::runtime_error("Failed to initialize GLEW.");
    }

    glew::glewExperimental=true; // ? is this needed

    // Ensure we can capture the escape key being pressed below
    glfw::glfwSetInputMode(ptr.get(), GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glew::glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

    glew::glGenVertexArrays(1, &VertexArrayID);
    glew::glBindVertexArray(VertexArrayID);
}

Viewer::Viewer()
:   window{WINDOW_WIDTH, WINDOW_HEIGHT},
    pipeline{std::string{"old"}}
{
    // programID = load_shaders();
}

Viewer::~Viewer() {
  glfw::glfwTerminate();
}

void Viewer::render(Scene &scene) {
  glew::glClear(GL_COLOR_BUFFER_BIT);
  glew::glUseProgram(pipeline.shaderProgramId);

  for (auto &&obj : scene.objects()) {
    auto *sphere = static_cast<Sphere*>(obj.get());
    drawCircle(sphere->position.x, sphere->position.y, sphere->radius);
  }
  
  glfw::glfwSwapBuffers(window.ptr.get());
  glfw::glfwPollEvents();
}

// todo - have a controller for pressed keys
bool Viewer::closeWindow()
{
  return !(glfw::glfwGetKey(window.ptr.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS and
            glfw::glfwWindowShouldClose(window.ptr.get()) == 0);
}

} // namespace fcm
