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
    shader{std::string{"default"}}  
{
    // Dark blue background
    glew::glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
}

Viewer::~Viewer() {
    glfw::glfwTerminate();
}

void Viewer::render(Scene &scene) {
  glew::glClear(GL_COLOR_BUFFER_BIT);
  glew::glEnable(GL_CULL_FACE);  
  

  glm::mat4 p = glm::perspective(glm::radians(60.0f), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.01f, 100.0f);

  glm::vec3 position{0.0, 0.0, 40.0};
  glm::vec3 target{0.0, 0.0, 0.0};
  glm::vec3 up{0.0, 1.0, 0.0};
  glm::mat4 v = glm::lookAt(position, target, up);

  shader.setUniform4fv("uColor", glm::vec4{1., 0., 1., 1.});
  shader.setUniform3fv("uLightPos", -1.5f*position);

  for (auto &&obj : scene.objects()) {
    // auto *sphere = static_cast<Sphere*>(obj.get());
    // drawCircle(sphere->position.x, sphere->position.y, sphere->radius);
    auto &mesh = obj->mesh;

    glm::mat4 t{1.f};
    t = glm::translate(t, obj->position);

    auto rx = obj->angular_position;
    glm::mat4 r = glm::eulerAngleYXZ(rx.y, rx.x, rx.z);

    glm::mat4 s = mesh.transform;

    glm::mat4 xform = s * t;

    
    glm::mat4 mvp = p * v * xform;

    shader.setUniformMat4fv("uMVP", mvp);
    shader.setUniformMat4fv("uM", xform);
    
    draw(mesh.meshData->va, mesh.meshData->ib);
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
