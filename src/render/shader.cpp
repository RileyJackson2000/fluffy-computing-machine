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
Shader::~Shader() { glDeleteProgram(handle); }

void Shader::bind() { glUseProgram(handle); }

void Shader::unbind() { glUseProgram(0); }

void Shader::setInt(const std::string &name, int i) {
  glUniform1i(getUniformLocation(name), i);
}

void Shader::setFloat(const std::string &name, float v) {
  glUniform1f(getUniformLocation(name), v);
}

void Shader::setVec3(const std::string &name, glm::vec3 v) {
  glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v));
}

void Shader::setVec4(const std::string &name, glm::vec4 v) {
  glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(v));
}

void Shader::setMat3(const std::string &name, glm::mat3 m) {
  glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::setMat4(const std::string &name, glm::mat4 m) {
  glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
}

GLHandle Shader::compileShader(const GLenum &shaderType,
                               const std::string &shaderSource) {
  GLuint shaderId{glCreateShader(shaderType)};

  const char *shaderData{shaderSource.c_str()};
  glShaderSource(shaderId, 1, &shaderData, nullptr);
  glCompileShader(shaderId);

  GLint shaderCompilationResult;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompilationResult);

  if (!shaderCompilationResult) {
    GLint errorMessageLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorMessageLength);
    std::vector<char> errorMessage(errorMessageLength + 1);
    glGetShaderInfoLog(shaderId, errorMessageLength, nullptr,
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

GLHandle Shader::createShader(const std::string &shaderName) {
  std::cout << "Initializing Shader: " << shaderName << std::endl;

  const std::string vertexShaderSource{
      loadTextFile("../src/render/shaders/" + shaderName + ".vert")};
  const std::string fragmentShaderSource{
      loadTextFile("../src/render/shaders/" + shaderName + ".frag")};

  GLuint ShaderId{glCreateProgram()};
  GLuint vertexShaderId{compileShader(GL_VERTEX_SHADER, vertexShaderSource)};
  GLuint fragmentShaderId{
      compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource)};

  glAttachShader(ShaderId, vertexShaderId);
  glAttachShader(ShaderId, fragmentShaderId);
  glLinkProgram(ShaderId);

  GLint ShaderLinkResult;
  glGetProgramiv(ShaderId, GL_LINK_STATUS, &ShaderLinkResult);

  if (!ShaderLinkResult) {
    GLint errorMessageLength;
    glGetProgramiv(ShaderId, GL_INFO_LOG_LENGTH, &errorMessageLength);
    std::vector<char> errorMessage(errorMessageLength + 1);
    glGetProgramInfoLog(ShaderId, errorMessageLength, nullptr,
                        &errorMessage[0]);

    std::cerr << errorMessage.data() << std::endl;
    throw std::runtime_error("Shader program failed to link.");
  }

  glDetachShader(ShaderId, vertexShaderId);
  glDetachShader(ShaderId, fragmentShaderId);
  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);

  return ShaderId;
}

GLint Shader::getUniformLocation(const std::string &uniformName) {
  if (uniformCache.find(uniformName) == uniformCache.end()) {
    GLint uniformLocation = glGetUniformLocation(handle, uniformName.c_str());
    uniformCache[uniformName] = uniformLocation;

    if (uniformLocation == -1) {
      std::cerr << "WARNING: Couldn't find uniform (" << uniformName
                << ") in shader (" << name << ")" << std::endl;
    }
  }
  return uniformCache[uniformName];
}

} // namespace fcm
