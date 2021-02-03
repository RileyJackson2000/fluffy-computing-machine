#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <render/shaders.hpp>

#include <utils/glew.hpp>

namespace fcm {

glew::GLuint load_shaders() {
  // hardcoded file paths for now
  char vertex_file_path[] = "../src/render/shaders/SimpleVertexShader.glsl";
  char fragment_file_path[] = "../src/render/shaders/SimpleFragmentShader.glsl";

  // Create the shaders
  glew::GLuint VertexShaderID = glew::glCreateShader(GL_VERTEX_SHADER);
  glew::GLuint FragmentShaderID = glew::glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if (VertexShaderStream.is_open()) {
    std::stringstream sstr;
    sstr << VertexShaderStream.rdbuf();
    VertexShaderCode = sstr.str();
    VertexShaderStream.close();
  } else {
    std::cout << vertex_file_path << " not found! :(\n";
    return 0;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if (FragmentShaderStream.is_open()) {
    std::stringstream sstr;
    sstr << FragmentShaderStream.rdbuf();
    FragmentShaderCode = sstr.str();
    FragmentShaderStream.close();
  } else {
    std::cout << fragment_file_path << " not found! :(\n";
    return 0;
  }

  glew::GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  std::cout << "Compiling shader: " << vertex_file_path << "\n";
  char const *VertexSourcePointer = VertexShaderCode.c_str();
  glew::glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glew::glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glew::glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glew::glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
    glew::glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,
                             &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }

  // Compile Fragment Shader
  std::cout << "Compiling shader: " << vertex_file_path << "\n";
  char const *FragmentSourcePointer = FragmentShaderCode.c_str();
  glew::glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glew::glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glew::glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glew::glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
    glew::glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL,
                             &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }

  // Link the program
  std::cout << "Linking program\n";
  glew::GLuint ProgramID = glew::glCreateProgram();
  glew::glAttachShader(ProgramID, VertexShaderID);
  glew::glAttachShader(ProgramID, FragmentShaderID);
  glew::glLinkProgram(ProgramID);

  // Check the program
  glew::glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glew::glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    glew::glGetProgramInfoLog(ProgramID, InfoLogLength, NULL,
                              &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }

  glew::glDetachShader(ProgramID, VertexShaderID);
  glew::glDetachShader(ProgramID, FragmentShaderID);

  glew::glDeleteShader(VertexShaderID);
  glew::glDeleteShader(FragmentShaderID);

  return ProgramID;
}

} // namespace fcm
