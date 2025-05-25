#include "shader.h"

GLuint Shader::compileShader(const char* shaderCode, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	// Проверка компиляции
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		std::string shaderName;
		switch (type) {
			case GL_VERTEX_SHADER:
				shaderName = "Vertex Shader";
			case GL_FRAGMENT_SHADER:
				shaderName = "Fragment Shader";
			default:
				shaderName = "Uknown Shader";
		}
		std::cout << "[SHADER:ERROR] " << shaderName << " не был скомпилирован: " << infoLog << std::endl;
	}

	return shader;
}

void Shader::createProgram(GLuint vertexShader, GLuint fragmentShader)
{
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	// Проверка линковки шейдерной программы к OpenGL
	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "[SHADER:ERROR] Shader program не была cформирована: " << infoLog << std::endl;
	}
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// Считывание с файла (не ебу как работает)
	std::string vCode;
	std::string fCode;

	std::ifstream vFile;
	std::ifstream fFile;

	vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vFile.open(vertexPath);
		fFile.open(fragmentPath);

		std::stringstream vStream, fStream;
		vStream << vFile.rdbuf();
		fStream << fFile.rdbuf();

		vFile.close();
		fFile.close();

		vCode = vStream.str();
		fCode = fStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "[SHADER:ERROR] Не удалось прочитать шейдерные файлы. " << std::endl;
	}

	const char* vShaderCode = vCode.c_str();
	const char* fShaderCode = fCode.c_str();

	// Созданем vertex и fragment shader object 
	GLuint vertexShader = compileShader(vShaderCode, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(fShaderCode, GL_FRAGMENT_SHADER);

	createProgram(vertexShader, fragmentShader);

	// Свое они уже изжили
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3f(const std::string& name, float xvalue, float yvalue, float zvalue) {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), xvalue, yvalue, zvalue);
}

void Shader::setVec4f(const std::string& name, float xvalue, float yvalue, float zvalue, float wvalue) {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), xvalue, yvalue, zvalue, wvalue);
}
