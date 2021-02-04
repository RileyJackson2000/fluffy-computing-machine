#pragma once

#include <utils/glfw.hpp>
#include <model/scene.hpp>
#include <render/GLPipeline.hpp>

namespace fcm {

glfw::GLFWwindow_ptr init_renderer(void);

void render(Scene &, glfw::GLFWwindow *);

void destroy_renderer(void);

// this is really scuffed - We need to load shaders after initializing window - TODO fix this
struct Window {
    // TODO resizable window?
    glfw::GLFWwindow_ptr ptr;
    Window(unsigned int width, unsigned int height);
};

struct Viewer {
    Window window;
    
    GLPipeline pipeline;

    Viewer();
    ~Viewer();

    void render(Scene &scene);
    bool closeWindow();
};

}
