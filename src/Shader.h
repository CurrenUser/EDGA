#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>


class Shader {
public:
	Shader(std::pair<const char, std::string> shaderSrcOne, std::pair<const char, std::string> shaderSrcTwo);
	~Shader ();
	GLuint shaderProgram;
	void Shader::Use();

private:
	std::string LoadShaderSrc(std::string name);
	void CreateShaderProgram();
	void CreateShader(const char shaderType, std::string name);
	GLuint vertexShader;
	GLuint fragmentShader;
	GLint success;
	GLchar infoLog[1024];
};

