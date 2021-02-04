#pragma once

#include <model/scene.hpp>
#include <utils/glfw.hpp>

namespace fcm {

glfw::GLFWwindow_ptr init_renderer(void);

void render(Scene &, glfw::GLFWwindow *);

void destroy_renderer(glfw::GLFWwindow_ptr);

} // namespace fcm
