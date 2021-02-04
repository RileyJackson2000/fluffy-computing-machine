#pragma once

#include <string>
#include <utils/glew.hpp>

namespace fcm {
    
struct GLPipeline
{
    const glew::GLuint shaderProgramId;
    const glew::GLuint uniformLocationMVP;
    const glew::GLuint attributeLocationVertexPosition;

    GLPipeline(const std::string &shaderName);
    ~GLPipeline();
};

} // namespace fcm
