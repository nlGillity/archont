#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader 
{
private:
	GLuint compileShader(const char* shaderCode, GLenum type);
	void createProgram(GLuint vertexShader, GLuint fragmentShader);

public:
	GLuint ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void unuse();

	// Установка значений для uniform-переменных
	void setUniform(const std::string& name, GLint value);
	void setUniform(const std::string& name, GLfloat value);
	void setUniform(const std::string& name, bool value);
	void setUniform(const std::string& name, glm::vec3 value);
	void setUniform(const std::string& name, glm::vec4 value);
	void setUniform(const std::string& name, GLfloat* mat);
};