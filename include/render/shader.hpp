#pragma once

#include <string>
#include <unordered_map>

#include <utils/glew.hpp>
#include <utils/glm.hpp>

namespace fcm {

struct Shader {
  std::string name;
  GLHandle handle;

  Shader(const std::string &shaderName);
  ~Shader();

  void bind();
  void unbind();

  // add setters here as necessary
  void setInt(const std::string &name, int v);
  void setFloat(const std::string &name, float v);
  void setVec3(const std::string &name, glm::vec3 v);
  void setVec4(const std::string &name, glm::vec4 v);
  void setMat3(const std::string &name, glm::mat3 m);
  void setMat4(const std::string &name, glm::mat4 m);

private:
  std::unordered_map<std::string, GLint> uniformCache;

  GLHandle compileShader(const GLenum &shaderType,
                         const std::string &shaderSource);
  std::string loadTextFile(const std::string &filepath);
  GLHandle createShader(const std::string &shaderName);
  GLint getUniformLocation(const std::string &uniformName);
};

} // namespace fcm
