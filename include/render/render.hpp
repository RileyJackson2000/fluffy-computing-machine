#pragma once

#include <utils/glfw.hpp>
#include <model/scene.hpp>
#include <render/shaderProgram.hpp>
#include <render/camera.hpp>

namespace fcm {

glfw::GLFWwindow_ptr init_renderer(void);

void render(Scene &, glfw::GLFWwindow *);

void destroy_renderer(void);

// this is really scuffed - We need to load shaders after initializing window - TODO fix this
// this shit causes a bunch of errors - we should figure out how to change this
struct Window {
    // TODO resizable window?
    glfw::GLFWwindow_ptr ptr;
    Window(unsigned int width, unsigned int height);
};

struct Viewer {
    Window window;
    ShaderProgram shader;
    Camera cam;

    Viewer();
    ~Viewer();

    void render(Scene &scene);

    void draw(const VertexArray &va, const IndexBuffer &ib);

    bool closeWindow();
};

}
