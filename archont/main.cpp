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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test", NULL, NULL);
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

	// ---------------------------------------------------------------

	// SHADER
	Shader shader("vertex.vert", "fragment.frag");
	
	// ENTITY
	vector<vertex> vertices =
	{
		vertex(vec3(-0.5f, -0.5f, 0.0f), vec3(1.0f, 0.0f, 0.0f)), // левая нижнняя
		vertex(vec3(0.5f, -0.5f, 0.0f),  vec3(0.0f, 1.0f, 0.0f)), // правая нижняя
		vertex(vec3(0.0f,  0.5f, 0.0f),  vec3(0.0f, 0.0f, 1.0f))  // верхняя
	};
	vector<GLuint> indices = { 0, 1, 2 };

	Entity triangle(vertices, indices);
	triangle.linkShader(&shader);

	// ---------------------------------------------------------------

	// ЦИКЛ РЕНДЕРИНГА
	while (!glfwWindowShouldClose(window))
	{
		// Создание заднего буфера (color buffer'а) для рендеринга
		glClear(GL_COLOR_BUFFER_BIT);

		// Отрисовка треугольника
		triangle.rotate((float)abs(glfwGetTime()), vec3(1.0f, 0.0f, 0.0f));
		triangle.render(GL_TRIANGLES, GL_UNSIGNED_INT);

		// Проверка различных вызовов от пользователя (клавиатура, мышка)
		glfwPollEvents();
		// Смена color buffer'ов (обновление цвета каждого пикселя)
		glfwSwapBuffers(window);
	}

	// Освобождение памяти (не обязательно)
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height)
{
	glViewport(0, 0, width, height);
}