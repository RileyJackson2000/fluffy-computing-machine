#include <fstream>
#include <iostream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <vector>

#include <utils/glew.hpp>

#include <render/shader.hpp>

namespace fcm {

Shader::Shader(const std::string &shaderName) : name{shaderName} {
  handle = createShader(shaderName);
}
Shader::~Shader() { glew::glDeleteProgram(handle); }

void Shader::bind() { glew::glUseProgram(handle); }

void Shader::unbind() { glew::glUseProgram(0); }

void Shader::setVec3(const std::string &name, glm::vec3 v) {
  glew::glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v));
}

void Shader::setVec4(const std::string &name, glm::vec4 v) {
  glew::glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(v));
}

void Shader::setMat4(const std::string &name, glm::mat4 m) {
  glew::glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                           glm::value_ptr(m));
}

glew::GLHandle Shader::compileShader(const glew::GLenum &shaderType,
                                     const std::string &shaderSource) {
  glew::GLuint shaderId{glew::glCreateShader(shaderType)};

  const char *shaderData{shaderSource.c_str()};
  glew::glShaderSource(shaderId, 1, &shaderData, nullptr);
  glew::glCompileShader(shaderId);

  glew::GLint shaderCompilationResult;
  glew::glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompilationResult);

  if (!shaderCompilationResult) {
    glew::GLint errorMessageLength;
    glew::glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorMessageLength);
    std::vector<char> errorMessage(errorMessageLength + 1);
    glew::glGetShaderInfoLog(shaderId, errorMessageLength, nullptr,
                             errorMessage.data());

    std::cerr << errorMessage.data() << std::endl;

    std::string type = shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
    throw std::runtime_error(type + " shader failed to compile.");
  }

  return shaderId;
}

// TODO move this to a utility file
std::string Shader::loadTextFile(const std::string &filepath) {
  std::string fileContents;
  std::ifstream in(filepath, std::ios::in);
  if (in.is_open())
    return std::string{(std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>()};

  std::cout << filepath << " not found!" << std::endl;
  return std::string{};
}

glew::GLHandle Shader::createShader(const std::string &shaderName) {
  std::cout << "Initializing Shader: " << shaderName << std::endl;

  const std::string vertexShaderSource{
      loadTextFile("../src/render/shaders/" + shaderName + ".vert")};
  const std::string fragmentShaderSource{
      loadTextFile("../src/render/shaders/" + shaderName + ".frag")};

  glew::GLuint ShaderId{glew::glCreateProgram()};
  glew::GLuint vertexShaderId{
      compileShader(GL_VERTEX_SHADER, vertexShaderSource)};
  glew::GLuint fragmentShaderId{
      compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource)};

  glew::glAttachShader(ShaderId, vertexShaderId);
  glew::glAttachShader(ShaderId, fragmentShaderId);
  glew::glLinkProgram(ShaderId);

  glew::GLint ShaderLinkResult;
  glew::glGetProgramiv(ShaderId, GL_LINK_STATUS, &ShaderLinkResult);

  if (!ShaderLinkResult) {
    glew::GLint errorMessageLength;
    glew::glGetProgramiv(ShaderId, GL_INFO_LOG_LENGTH, &errorMessageLength);
    std::vector<char> errorMessage(errorMessageLength + 1);
    glew::glGetProgramInfoLog(ShaderId, errorMessageLength, nullptr,
                              &errorMessage[0]);

    std::cerr << errorMessage.data() << std::endl;
    throw std::runtime_error("Shader program failed to link.");
  }

  glew::glDetachShader(ShaderId, vertexShaderId);
  glew::glDetachShader(ShaderId, fragmentShaderId);
  glew::glDeleteShader(vertexShaderId);
  glew::glDeleteShader(fragmentShaderId);

  return ShaderId;
}

glew::GLint Shader::getUniformLocation(const std::string &uniformName) {
  if (uniformCache.find(uniformName) == uniformCache.end()) {
    glew::GLint uniformLocation =
        glew::glGetUniformLocation(handle, uniformName.c_str());
    uniformCache[uniformName] = uniformLocation;

    if (uniformLocation == -1) {
      std::cerr << "WARNING: Couldn't find uniform (" << uniformName
                << ") in shader (" << name << ")" << std::endl;
    }
  }
  return uniformCache[uniformName];
}

} // namespace fcm
