#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

// Шейдеры мои шейдеры
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 fragColor;\n"
"uniform vec4 outColor;\n"
"void main()\n"
"{\n"
"   fragColor = outColor;\n"
"}\0";

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

int main() {
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
	if (window == nullptr) {
		cout << "[ERROR] Не удалось создать окно программы" << endl;
		glfwTerminate();
		return 1;
	}
	// Создаем контекст для OpenGL
	glfwMakeContextCurrent(window);

	// ИНИЦИАЛИЗАЦИЯ GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "[ERROR] Не удалось инициализировать GLAD" << endl;
		return 1;
	}

	// ОБЛАСТЬ РЕНДЕРИНГА для OpenGL (на все окно программы)
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// ---------------------------------------------------------------

	// Вершины треугольника
	float vertices[] = {
		// Координаты вершин:	// Цвет вершин:
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, // левая нижнняя
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, // правая нижняя
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f  // верхняя

	};
	// Индексы выршин (поряд их отрисовки)
	unsigned int indices[] = {
		0, 1, 2
	};

	// VAO
	// Создаем новый vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	// Создаем новый буфер vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// Привызываем его к GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Копируем данные (коодринаты вершин) в vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO
	// Создаем новый буфер element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// Привызываем его к GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Копируем данные (порядок отрисовки вершин) в vertex buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/** РАЗМЕТКА АТТРИБУТОВ
		index: индекс аттрибута (location = 0)
		size: размер аттрибута (vec3)
		type: тип данных (float)
		normilized: нормировка данных (false, т.к. координаты уже от -1 до 1)
		stride: размер сдвига к аттрибутам следующей вершины
		pointer: начальное смещение до первого аттрибута данного индекса
	**/
	// Аттрибут позиции
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Аттрибут цвета
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// VERTEX SHADER
	// Созданем shader object (vertex)
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Прикрепляем код вершинного шейдера
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Компиляция вершинного шейдера
	glCompileShader(vertexShader);
	// Проверка компиляции
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "[ERROR] Vertex shader не был скомпилирован: " << infoLog << endl;
	}

	// FRAGMENT SHADER
	// Созданем shader object (fragment)
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Прикрепляем код вершинного шейдера
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Компиляция вершинного шейдера
	glCompileShader(fragmentShader);
	// Проверка компиляции
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "[ERROR] Fragment shader не был скомпилирован: " << infoLog << endl;
	}

	// SHADER PROGRAM
	// Создаем program object
	unsigned int shaderProgram = glCreateProgram();
	// Связываем шейдеры с шейдерной программой
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Проверка линковки шейдерной программы к OpenGL
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "[ERROR] Shader program не была cформирована: " << infoLog << endl;
	}

	// Свое они уже изжили
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ---------------------------------------------------------------

	// ЦИКЛ РЕНДЕРИНГА
	while (!glfwWindowShouldClose(window)) {
		// Создание заднего буфера (color buffer'а) для рендеринга
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		float timeValue = glfwGetTime();
		float redOscillation = sin(2 * (timeValue)+3.14 / 2) + 1.0f;
		float greenOscillation = sin(2 * (timeValue)) + 1.0f;
		float blueOscillation = sin(2 * (timeValue)+3.14) + 1.0f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "outColor");
		glUniform4f(vertexColorLocation, redOscillation, greenOscillation, blueOscillation, 1.0f);

		// Отрисовка треугольника
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);

		// Проверка различных вызовов от пользователя (клавиатура, мышка)
		glfwPollEvents();
		// Смена color buffer'ов (обновление цвета каждого пикселя)
		glfwSwapBuffers(window);
	}

	// Освобождение памяти (не обязательно)
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}