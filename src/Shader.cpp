#include "Shader.h"

Shader::Shader(std::pair<const char, std::string> shaderSrcOne, std::pair<const char, std::string> shaderSrcTwo) {
    this->CreateShader(shaderSrcOne.first, shaderSrcOne.second);
    this->CreateShader(shaderSrcTwo.first, shaderSrcTwo.second);
    this->CreateShaderProgram();
}

Shader ::~Shader() {
    glDeleteProgram(shaderProgram);
}

void Shader::Use() {
    glUseProgram(shaderProgram);
}

void Shader::CreateShader(const char shaderType, std::string name) {
    std::string temp_str= this->LoadShaderSrc(name);
    const GLchar* src = temp_str.c_str();
    GLint shader;

    switch (shaderType) {
    case 'f':
        std::cout << "Compile fragment shader\n";
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        fragmentShader = shader;
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);
        break;
    case 'v':
        std::cout << "Compile vertex shader\n";
        shader = glCreateShader(GL_VERTEX_SHADER);
        vertexShader = shader;
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);
        break;
    default:
        std::cout << "Error, unknown shader type\n";
        break;
    }
}

void Shader::CreateShaderProgram() {
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        std::cout << "Error linked shader program!\n" << infoLog << "\n";
    }
}

std::string Shader::LoadShaderSrc(std::string name) {
    std::ifstream inf { name };
    if (inf) {
        std::string buffer, shaderSrc{};
            while (std::getline(inf, buffer)) {
                shaderSrc += buffer + "\n";
            }
        inf.close();
        return shaderSrc;
    }
    else {
       std::cerr << "Error open file " << name << " for read\n";
       return std::string();
    }
}
