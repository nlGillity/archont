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

	// Установка значений для uniform-переменных
	void setInt(const std::string& name, GLint value);
	void setFloat(const std::string& name, GLfloat value);
	void setBool (const std::string& name, bool value);
	void setVec3f(const std::string& name, glm::vec3 vec);
	void setVec4f(const std::string& name, glm::vec4 vec);
	void setMat4f(const std::string& name, GLfloat* adrmat);
};