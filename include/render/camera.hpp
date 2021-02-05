#pragma once

#include <utils/glew.hpp>
#include <utils/glm.hpp>

namespace fcm {
    
struct Camera
{
    glm::mat4 projMat, viewMat;

    Camera(float aspect)
    {
        setProjMat(glm::radians(60.0f), aspect);

        glm::vec3 position{0.0, 0.0, 30.0};
        glm::vec3 target{0.0, 0.0, 0.0};
        glm::vec3 up{0.0, 1.0, 0.0};
        setViewMat(position, target, up);
    }

    void setProjMat(float fov, float aspect)
    {
        projMat = glm::perspective(fov, aspect, 0.01f, 100.0f);
    }

    void setViewMat(glm::vec3 from, glm::vec3 to, glm::vec3 up)
    {
        viewMat = glm::lookAt(from, to, up);
    }
};

} // namespace fcm
