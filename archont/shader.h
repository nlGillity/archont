#pragma once

#include <glad/glad.h>

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
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setBool (const std::string& name, bool value);
	void setVec3f(const std::string& name, float xvalue, float yvalue, float zvalue);
	void setVec4f(const std::string& name, float xvalue, float yvalue, float zvalue, float wvalue);
};