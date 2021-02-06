#include <model/object.hpp>
#include <render/render.hpp>

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
    glfw::glfwSetInputMode(ptr.get(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

float lastMouseX = WINDOW_WIDTH / 2.f;
float lastMouseY = WINDOW_HEIGHT / 2.f;
float yaw = -90.f;
float pitch = 0;
float fov = 60.f;
float mouseSensitivity = 0.1;
glm::vec3 camDir{0.f,0.f,-1.f};

static void mouse_callback(glfw::GLFWwindow *window, double xpos, double ypos)
{
    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    glm::clamp(pitch, -89.f, 89.f);
    camDir.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    camDir.y = glm::sin(glm::radians(pitch));
    camDir.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    camDir = glm::normalize(camDir);
}

static void scroll_callback(glfw::GLFWwindow *window, double xoff, double yoff)
{
    fov -= yoff;
    glm::clamp(fov, 1.f, 60.f);
}

Viewer::Viewer()
:   window{WINDOW_WIDTH, WINDOW_HEIGHT},
    shader{std::string{"default"}},
    cam{float(WINDOW_WIDTH) / float(WINDOW_HEIGHT)}
{
    // Dark blue background
    glew::glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

    glfw::glfwSetCursorPosCallback(window.ptr.get(), mouse_callback);
    glfw::glfwSetScrollCallback(window.ptr.get(), scroll_callback);
    lastFrameTime = getTime();
}

Viewer::~Viewer() {
    glfw::glfwTerminate();
}

void Viewer::render(Scene &scene) {
    // controller
    pollEvents();
    cam.fov = fov;
    cam.dir = camDir;
    double currentTime = getTime();
    double dt = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    updateCameraPos(dt);


    glew::glClear(GL_COLOR_BUFFER_BIT);
    // glew::glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glew::glEnable(GL_CULL_FACE);  

    shader.setVec4("uColor", glm::vec4{1., 0., 1., 1.});
    shader.setVec3("uLightPos", glm::vec3{1, 2, 300});
    shader.setMat4("uV", cam.viewMat());
    shader.setMat4("uP", cam.projectionMat());

    for (auto &&obj : scene.objects()) {
        shader.setMat4("uM", obj->getTransform());
        draw(obj->glMeshData);
    }

    glfw::glfwSwapBuffers(window.ptr.get());

  
}

void Viewer::draw(const GLMeshData &glMeshData)
{
    shader.bind();
    glMeshData.va.bind();
    glMeshData.ib.bind();
    glew::glDrawElements(GL_TRIANGLES, glMeshData.ib.numIndices, GL_UNSIGNED_INT, nullptr);
}

void Viewer::updateCameraPos(double dt)
{
    float speed;
    if (window.getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = movementSpeed * 0.33;
    else
        speed = movementSpeed;

    float multiplier = speed * (dt * 1000.f);
    if (window.getKey(GLFW_KEY_W) == GLFW_PRESS)
        cam.pos += multiplier * cam.dir;
    if (window.getKey(GLFW_KEY_S) == GLFW_PRESS)
        cam.pos -= multiplier * cam.dir;
    if (window.getKey(GLFW_KEY_A) == GLFW_PRESS)
        cam.pos -= multiplier * glm::normalize(glm::cross(cam.dir, cam.up));
    if (window.getKey(GLFW_KEY_D) == GLFW_PRESS)
        cam.pos += multiplier * glm::normalize(glm::cross(cam.dir, cam.up));
}

// todo - have a controller for pressed keys
bool Viewer::closeWindow()
{   
    if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
        window.setShouldClose(true);
    return window.shouldClose();
}

} // namespace fcm
