#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // Read vertex shader and fragment shader source code from files
    vertexCodePtr = readShaderSource(vertexPath);
    fragmentCodePtr = readShaderSource(fragmentPath);

    // Convert string pointers to C-style strings required by OpenGL
    const char* vShaderCode = vertexCodePtr->c_str();
    const char* fShaderCode = fragmentCodePtr->c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Create vertex shader object and compile it
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Create fragment shader object and compile it
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Create shader program object and attach vertex and fragment shaders to it
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    // Link shader program
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Delete vertex and fragment shaders as they are now linked into the program object and no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::use() const {
    glUseProgram(ID);
}

unsigned int Shader::getID() const {
    return ID;
}

std::shared_ptr<std::string> Shader::readShaderSource(const std::string& filePath) {
    std::ifstream shaderFile;
    shaderFile.open(filePath);
    if (!shaderFile.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return nullptr;
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    return std::make_shared<std::string>(shaderStream.str());
}

void Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
