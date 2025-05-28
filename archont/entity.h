#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <iostream>
#include <vector>
using namespace glm;
using namespace std;

struct vertex
{
	glm::vec3 position;
	glm::vec3 color;

	vertex(glm::vec3 _position, glm::vec3 _color):
		position(_position), color(_color) {}
};

class Entity
{
protected:
	std::vector<vertex> vertices;
	std::vector<GLuint> indices;
	GLuint VAO, VBO, EBO;

	glm::mat4 transform;
	Shader* shader;

public:
	Entity(std::vector<vertex> &vertices, std::vector<GLuint> &indices);
	~Entity();

	void linkShader(Shader* shader);
	void render(GLenum mode, GLenum type);

	void rotate(float degree, glm::vec3 axis);
	void translate(glm::vec3 direction);
};