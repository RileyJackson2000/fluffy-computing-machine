#pragma once

#include <string>
#include <unordered_map>

#include <utils/glew.hpp>
#include <utils/glm.hpp>

namespace fcm {
    
struct ShaderProgram
{
    std::string name;
    glew::GLHandle handle;

    ShaderProgram(const std::string &shaderName);
    ~ShaderProgram();

    void bind();
    void unbind();

    // add setters here as necessary
    void setUniform3fv(const std::string &name, glm::vec3 v);
    void setUniform4fv(const std::string &name, glm::vec4 v);
    void setUniformMat4fv(const std::string &name, glm::mat4 m);

private:
    std::unordered_map<std::string, glew::GLint> uniformCache;

    glew::GLHandle compileShader(const glew::GLenum& shaderType, const std::string& shaderSource);
    std::string loadTextFile(const std::string& filepath);
    glew::GLHandle createShaderProgram(const std::string& shaderName);
    glew::GLint getUniformLocation(const std::string& uniformName);
};

} // namespace fcm
