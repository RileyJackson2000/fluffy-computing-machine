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

    std::cout << "OpenGL Version: " << glew::glGetString(GL_VERSION) << std::endl;


    // Ensure we can capture the escape key being pressed below
    glfw::glfwSetInputMode(ptr.get(), GLFW_STICKY_KEYS, GL_TRUE);
}

Viewer::Viewer()
:   window{WINDOW_WIDTH, WINDOW_HEIGHT},
    shader{std::string{"default"}},
    cam{float(WINDOW_WIDTH) / float(WINDOW_HEIGHT)}
{
    // Dark blue background
    glew::glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
}

Viewer::~Viewer() {
    glfw::glfwTerminate();
}

void Viewer::render(Scene &scene) {
  glew::glClear(GL_COLOR_BUFFER_BIT);
//   glew::glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  glew::glEnable(GL_CULL_FACE);  
  
  shader.setUniform4fv("uColor", glm::vec4{1., 0., 1., 1.});
  shader.setUniform3fv("uLightPos", glm::vec3{0, -10, 30});
  shader.setUniformMat4fv("uV", cam.viewMat);
  shader.setUniformMat4fv("uP", cam.projMat);

  for (auto &&obj : scene.objects()) {
    shader.setUniformMat4fv("uM", obj->getTransform());
    draw(obj->glMeshData.va, obj->glMeshData.ib);
  }
  
  glfw::glfwSwapBuffers(window.ptr.get());
  glfw::glfwPollEvents();
}

void Viewer::draw(const VertexArray &va, const IndexBuffer &ib)
{
    shader.bind();
    va.bind();
    ib.bind();
    glew::glDrawElements(GL_TRIANGLES, ib.numIndices, GL_UNSIGNED_INT, nullptr);
}

// todo - have a controller for pressed keys
bool Viewer::closeWindow()
{
  return !(glfw::glfwGetKey(window.ptr.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS and
            glfw::glfwWindowShouldClose(window.ptr.get()) == 0);
}

} // namespace fcm
