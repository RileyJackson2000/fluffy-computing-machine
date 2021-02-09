#include <model/object.hpp>
#include <render/render.hpp>

#include <iostream>
#include <utils/constants.hpp>
#include <utils/glew.hpp>

#include <cmath>

namespace {
static void error_callback(int error, const char *description) {
  (void)error;
  std::cout << "Error: " << description << "\n";
}
} // namespace

namespace fcm {

Window::Window(uint32_t width, uint32_t height) {
  // Initialise GLFW
  if (!glfw::glfwInit()) {
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
      glfw::glfwCreateWindow(width, height, WINDOW_TITLE, NULL, NULL)};

  if (ptr.get() == nullptr) {
    glfw::glfwTerminate();
    throw std::runtime_error(
        "Failed to open GLFW window. See error logs for more info.");
  }

  glfw::glfwMakeContextCurrent(ptr.get());

  // initialize glew
  if (glew::glewInit() != GLEW_OK) {
    glfw::glfwTerminate();
    throw std::runtime_error("Failed to initialize GLEW.");
  }

  glew::glewExperimental = true; // ? is this needed

  std::cout << "OpenGL Version: " << glew::glGetString(GL_VERSION) << std::endl;

  glew::glEnable(GL_DEPTH_TEST);
  glew::glEnable(GL_CULL_FACE);

  // Ensure we can capture the escape key being pressed below
  if (glfw::glfwRawMouseMotionSupported())
    glfw::glfwSetInputMode(ptr.get(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  glfw::glfwSetInputMode(ptr.get(), GLFW_STICKY_KEYS, GL_TRUE);
  //glfw::glfwSetInputMode(ptr.get(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  // TEMP
  glew::GLuint VertexArrayID;
  glew::glGenVertexArrays(1, &VertexArrayID);
  glew::glBindVertexArray(VertexArrayID);
}

float lastMouseX = WINDOW_WIDTH / 2.f;
float lastMouseY = WINDOW_HEIGHT / 2.f;
float yaw = -90.f;
float pitch = 0;
float fov = 60.f;
float mouseSensitivity = 0.1;
glm::vec3 camDir{0.f, 0.f, -1.f};

static void mouse_callback(glfw::GLFWwindow *window, double xpos, double ypos) {
  (void)window;

  lastMouseX = WINDOW_WIDTH/2.;
  lastMouseY = WINDOW_HEIGHT/2.;
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

  glfw::glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

static void scroll_callback(glfw::GLFWwindow *window, double xoff,
                            double yoff) {
  (void)window;
  (void)xoff;

  fov -= yoff;
  glm::clamp(fov, 1.f, 60.f);
}

Viewer::Viewer()
    : window{WINDOW_WIDTH, WINDOW_HEIGHT}, shader{std::string{"default"}},
      cam{float(WINDOW_WIDTH) / float(WINDOW_HEIGHT)} {
  // Dark blue background
  glew::glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

  glfw::glfwSetCursorPosCallback(window.ptr.get(), mouse_callback);
  glfw::glfwSetScrollCallback(window.ptr.get(), scroll_callback);
  lastFrameTime = getTime();
}

Viewer::~Viewer() { glfw::glfwTerminate(); }

void Viewer::render(GLScene *scene) {
  // controller
  pollEvents();
  cam.fov = fov;
  cam.dir = camDir;
  double currentTime = getTime();
  double dt = currentTime - lastFrameTime;
  lastFrameTime = currentTime;
  updateCameraPos(dt);

  glew::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glew::glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  shader.setVec4("uColor", glm::vec4{1., 0., 1., 1.});
  shader.setVec3("uLightPos", glm::vec3{1, 2, 300});
  shader.setMat4("uV", cam.viewMat());
  shader.setMat4("uP", cam.projectionMat());

  // auto md = genSphereMesh(1, 10, 10, true);
  // GLMeshData glmd{md.get()};
  shader.setMat4("uM", glm::mat4{1.f});
  draw(scene->globjects()[0].glMeshData);

  //for (auto &&obj : scene->globjects()) {
  // for (int i = 0; i < scene->globjects().size(); ++i) {
  //   auto &&obj = scene->objects()[i];
  //   auto &&globj = scene->globjects()[i];
  //   shader.setMat4("uM", globj.getTransform());
  //   draw(obj->glMeshData);
    //draw(globj.glMeshData);
  //globj.glMeshData.ib.bind();
  //glew::glDrawElements(GL_TRIANGLES, globj.glMeshData.ib.numIndices, GL_UNSIGNED_INT,
                       //nullptr);
                        //This will identify our vertex buffer
  // std::vector<glm::vec3> verts;  
  // for (int j=0; j<obj->glMeshData.mesh->vertices.size(); ++j) {
  // verts.push_back(obj->meshData->vertices[j].pos);
  // }
  // glew::GLuint vertexbuffer;
  //                       // Generate 1 buffer, put the resulting identifier in vertexbuffer
  // glew::glGenBuffers(1, &vertexbuffer);
  //                       // The following commands will talk about our 'vertexbuffer' buffer
  // glew::glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  //                       // Give our vertices to OpenGL.
  // glew::glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*verts.size(), &verts, GL_STATIC_DRAW);
  // glew::glEnableVertexAttribArray(0);
  // glew::glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // glew::glVertexAttribPointer(
  //        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
  //       3,                  // size
  //          GL_FLOAT,           // type
  //             GL_FALSE,           // normalized?
  //                0,                  // stride
  //                   (void*)0            // array buffer offset
                        
  //     );
  // glew::glDrawArrays(GL_TRIANGLES, 0, obj->glMeshData.mesh_indices.size());
  // glew::glDisableVertexAttribArray(0);
  // }

  glfw::glfwSwapBuffers(window.ptr.get());
}

void Viewer::draw(const GLMeshData &glMeshData) {
  shader.bind();
  //std::cout <<shader.handle<<"\n";
  glMeshData.va.bind();
  //std::cout <<glMeshData.va.handle<<"\n";
  glMeshData.ib.bind();
  //std::cout <<glMeshData.ib.handle<<"\n";
  //std::cout <<glMeshData.ib.numIndices<<"\n";
  glew::glDrawElements(GL_TRIANGLES, glMeshData.ib.numIndices, GL_UNSIGNED_INT,
                       nullptr);
}

void Viewer::updateCameraPos(double dt) {
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
bool Viewer::closeWindow() {
  if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    window.setShouldClose(true);
  return window.shouldClose();
}

} // namespace fcm
