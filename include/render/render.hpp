#pragma once

#include <utils/glfw.hpp>

#include <model/scene.hpp>
#include <render/shader.hpp>
#include <render/camera.hpp>
#include <render/GLMesh.hpp>

namespace fcm {

// this is really scuffed - We need to load shaders after initializing window - TODO fix this
// this shit causes a bunch of errors - we should figure out how to change this
struct Window {
    // TODO resizable window?
    glfw::GLFWwindow_ptr ptr;
    Window(uint32_t width, uint32_t height);
    
    int getKey(int key) { return glfw::glfwGetKey(ptr.get(), key); }
    void setShouldClose(bool b) { glfw::glfwSetWindowShouldClose(ptr.get(), b); }
    bool shouldClose() { return glfw::glfwWindowShouldClose(ptr.get()); }
};

struct Viewer {
    Window window;
    Shader shader; // shaders should be part of materials. We should also support more than one shader
    Camera cam;

    Viewer();
    ~Viewer();

    void render(Scene &scene);
    void draw(const GLMeshData &glMeshData);

    // controller - TODO: move this to separete class
    float movementSpeed = 0.1;
    double lastFrameTime;

    void updateCameraPos(double dt);
    bool closeWindow();
    
    void pollEvents() { glfw::glfwPollEvents(); }
    double getTime() { return glfw::glfwGetTime(); }
};

}
