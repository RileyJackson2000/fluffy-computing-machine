#pragma once

#include <utils/glfw.hpp>
#include <model/scene.hpp>

namespace fcm {

glfw::GLFWwindow_ptr init_renderer(void);

void render(Scene &, glfw::GLFWwindow *);

void destroy_renderer(void);

}
