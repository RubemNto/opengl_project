#include <iostream>
#include <vector>
#include <string.h>

// #include "../header/House.h"
#include "../header/Model.h"
using namespace RenderEngine;

#include <time.h>
// File manipulation lib
// #include <fstream>

// #include "../header/Model.h"
// using namespace RenderEngine;

// callback functions
void print_error(int count, const char *desc);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset);

// constants of the program
const int WIDTH = 1280;
const int HEIGHT = 720;
const float ASPECT_RATIO = WIDTH / (float)HEIGHT;

int main()
{
	GLFWwindow *window;

	glfwSetErrorCallback(print_error);

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL - Iron Man", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	const Camera camera = Camera(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::radians(60.0f), ASPECT_RATIO, 0.1f, 1000.0f);
	Model model = Model("assets/iron_man/Iron_Man.obj");
	float rotation = 0;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		model.Draw(camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
		model.Draw(camera, glm::vec3(-2.0f, -1.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
		model.Draw(camera, glm::vec3(2.0f, -1.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
		glfwSwapBuffers(window);
		glfwPollEvents();
		rotation += 0.1f;
	}

	glfwTerminate();
	return 0;
}

void print_error(int count, const char *desc)
{
	std::cout << desc << std::endl;
}
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	// zoom -= yoffset;
}

void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	// pitch = -yoffset / HEIGHT * 360;
	// yaw = -xoffset / WIDTH * 360;
}