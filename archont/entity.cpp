#include "entity.h"

Entity::Entity(std::vector<vertex> &vertices, std::vector<GLuint> &indices) :
	transform(mat4(1.0f)), shader(nullptr)
{
	this->vertices = vertices;
	this->indices = indices;
		
	// Vertex Array Object
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	// Vertex Buffer Object
	glGenBuffers(1, &this->VBO);
	// Привызываем его к GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	// Копируем данные (атрибуты) в vertex buffer
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(vertex), &this->vertices[0], GL_STATIC_DRAW);

	// Element Buffer Object
	glGenBuffers(1, &this->EBO);
	// Привызываем его к GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	// Копируем данные (порядок отрисовки вершин) в element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// РАЗМЕТКА АТРИБУТОВ
	// Аттрибут позиции
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, position));
	glEnableVertexAttribArray(0);
	// Аттрибут цвета
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, color));
	glEnableVertexAttribArray(1);

	// Открепляемся от VBO, VAO, EBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Entity::~Entity()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2 &VBO, &EBO);
}

void Entity::linkShader(Shader* shader)
{
	this->shader = shader;
}

void Entity::render(GLenum mode, GLenum type)
{
	this->shader->use();
	this->shader->setUniform("transform", glm::value_ptr(transform));

	glBindVertexArray(this->VAO);
	glDrawElements(mode, this->indices.size(), type, 0);

	this->transform = glm::mat4(1.0f);
	glBindVertexArray(0);
	this->shader->unuse();
}

void Entity::rotate(float rad, glm::vec3 axis)
{
	transform = glm::rotate(transform, rad, axis);
}

void Entity::translate(glm::vec3 direction)
{
	transform = glm::translate(transform, direction);
}
