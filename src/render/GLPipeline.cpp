#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <streambuf>

#include <render/GLPipeline.hpp>

#include <utils/glew.hpp>

namespace
{

glew::GLuint compileShader(const glew::GLenum& shaderType, const std::string& shaderSource)
{        
    glew::GLuint shaderId{glew::glCreateShader(shaderType)};
    

    const char* shaderData{shaderSource.c_str()};
    glew::glShaderSource(shaderId, 1, &shaderData, nullptr);
    glew::glCompileShader(shaderId);

    glew::GLint shaderCompilationResult;
    glew::glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompilationResult);

    if (!shaderCompilationResult)
    {
        glew::GLint errorMessageLength;
        glew::glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorMessageLength);
        std::vector<char> errorMessage(errorMessageLength + 1);
        glew::glGetShaderInfoLog(shaderId, errorMessageLength, nullptr, errorMessage.data());

        std::cerr << errorMessage.data() << std::endl;
        throw std::runtime_error("Shader failed to compile.");
    }

    return shaderId;
}

// TODO move this to a utility file
std::string loadTextFile(const std::string& filepath)
{
    std::string fileContents;
    std::ifstream in(filepath, std::ios::in);
    if (in.is_open()) {
        return std::string{(std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()};
    } else {
        std::cout << filepath << " not found!" << std::endl;;
    }
}

glew::GLuint createShaderProgram(const std::string& shaderName)
{
    std::cout << "Initializing Shader: " << shaderName << std::endl;

    const std::string vertexShaderSource{loadTextFile("../src/render/shaders/" + shaderName + ".vert")};
    const std::string fragmentShaderSource{loadTextFile("../src/render/shaders/" + shaderName + ".frag")};

    glew::GLuint shaderProgramId{glew::glCreateProgram()};
    glew::GLuint vertexShaderId{::compileShader(GL_VERTEX_SHADER, vertexShaderSource)};
    glew::GLuint fragmentShaderId{::compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource)};

    glew::glAttachShader(shaderProgramId, vertexShaderId);
    glew::glAttachShader(shaderProgramId, fragmentShaderId);
    glew::glLinkProgram(shaderProgramId);

    glew::GLint shaderProgramLinkResult;
    glew::glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &shaderProgramLinkResult);

    if (!shaderProgramLinkResult)
    {
        glew::GLint errorMessageLength;
        glew::glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &errorMessageLength);
        std::vector<char> errorMessage(errorMessageLength + 1);
        glew::glGetProgramInfoLog(shaderProgramId, errorMessageLength, nullptr, &errorMessage[0]);
        
        std::cerr << errorMessage.data() << std::endl;
        throw std::runtime_error("Shader program failed to link.");
    }

    glew::glDetachShader(shaderProgramId, vertexShaderId);
    glew::glDetachShader(shaderProgramId, fragmentShaderId);
    glew::glDeleteShader(vertexShaderId);
    glew::glDeleteShader(fragmentShaderId);

    return shaderProgramId;
}

} // namespace

namespace fcm {

GLPipeline::GLPipeline(const std::string& shaderName)
:   shaderProgramId(::createShaderProgram(shaderName)),
    uniformLocationMVP(glew::glGetUniformLocation(shaderProgramId, "mvp")),
    attributeLocationVertexPosition(glew::glGetAttribLocation(shaderProgramId, "position")) {}
GLPipeline::~GLPipeline()
{
    glew::glDeleteProgram(shaderProgramId);
}

} // namespace fcm
