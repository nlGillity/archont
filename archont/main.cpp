#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "entity.h"

#include <iostream>
#include <vector>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height);

int main()
{
	// ИНИЦИАЛИЗАЦИЯ ОКНА ПРОГРАММЫ
	glfwInit();
	/**
		КОНФИГУРАЦИЯ ОКНА ПРОГРАММЫ
		Полный список параметроы:
			https://www.glfw.org/docs/latest/window.html#window_hints
	**/
	// Для совместимости со старными драйверами используем min возможную версию OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Используем более низкоуровнений инструментарий OpenGL (Core)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// СОЗДАНИЕ ОКНА ПРОГРАММЫ
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ArchEngine", NULL, NULL);
	if (window == nullptr)
	{
		cout << "[ERROR] Не удалось создать окно программы" << endl;
		glfwTerminate();
		return 1;
	}
	// Создаем контекст для OpenGL
	glfwMakeContextCurrent(window);

	// ИНИЦИАЛИЗАЦИЯ GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "[ERROR] Не удалось инициализировать GLAD" << endl;
		return 1;
	}

	// ОБЛАСТЬ РЕНДЕРИНГА для OpenGL (на все окно программы)
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glEnable(GL_DEPTH_TEST);

	// ---------------------------------------------------------------

	// SHADER
	Shader shader("vertex.vert", "fragment.frag");
	
	// TRIANGLE
	vector<vertex> vertices =
	{
		vertex(vec3(-0.5f, -0.5f, 0.0f), vec3(1.0f, 0.0f, 0.0f)), // левая нижнняя
		vertex(vec3(0.5f, -0.5f, 0.0f),  vec3(0.0f, 1.0f, 0.0f)), // правая нижняя
		vertex(vec3(0.0f,  0.5f, 0.0f),  vec3(0.0f, 0.0f, 1.0f))  // верхняя
	};
	vector<GLuint> indices = { 0, 1, 2 };

	Entity triangle(vertices, indices);
	triangle.linkShader(&shader);

	// PLANAR
	const int RESOLUTION_X = 100;
	const int RESOLUTION_Z = 100;

	vector<vertex> planarVertices;
	for (int z = 0; z < RESOLUTION_Z; z++) {
		for (int x = 0; x < RESOLUTION_X; x++) {
			planarVertices.push_back(vertex(
				vec3(2 * float(x) / RESOLUTION_X - 1, 0.0f, 2 * float(z) / RESOLUTION_Z - 1),
				vec3(1.0f, 1.0f, 1.0f))
			);
		}
	}

	vector<GLuint> planarIndices;
	for (int z = 0; z < RESOLUTION_Z - 1; z++) {
		for (int x = 0; x < RESOLUTION_X - 1; x++) {
			GLuint current = z * RESOLUTION_X + x;
			GLuint future = (z + 1) * RESOLUTION_X + x;

			GLuint i0 = current;
			GLuint i1 = current + 1;
			GLuint i2 = future;
			GLuint i3 = future + 1;

			if ((x + z) % 2) {
				planarIndices.push_back(i0); planarIndices.push_back(i1); planarIndices.push_back(i3);
				planarIndices.push_back(i3); planarIndices.push_back(i2); planarIndices.push_back(i0);
			}
			else {
				planarIndices.push_back(i0); planarIndices.push_back(i2); planarIndices.push_back(i1);
				planarIndices.push_back(i1); planarIndices.push_back(i3); planarIndices.push_back(i2);
			}
		}
	}

	Entity planar(planarVertices, planarIndices);
	planar.linkShader(&shader);

	// ---------------------------------------------------------------

	shader.use();
	// Настройка uniform-переменных
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
	view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

	shader.setUniform("view", glm::value_ptr(view));
	shader.setUniform("projection", glm::value_ptr(projection));

	// ЦИКЛ РЕНДЕРИНГА	
	while (!glfwWindowShouldClose(window))
	{
		// Создание заднего буфера (color buffer'а) для рендеринга
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Рендер плоскости
		shader.setUniform("time", (float)glfwGetTime());

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		planar.rotate(5 * glfwGetTime(), vec3(0.0f, 1.0f, 0.0f));
		planar.render(GL_TRIANGLES, GL_UNSIGNED_INT);

		// Проверка различных вызовов от пользователя (клавиатура, мышка)
		glfwPollEvents();
		// Смена color buffer'ов (обновление цвета каждого пикселя)
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height)
{
	glViewport(0, 0, width, height);
}