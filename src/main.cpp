#include <iostream>
#include <vector>
#include <string.h>
#include <time.h>

#include "../header/Model.h"
using namespace RenderEngine;

// callback functions
void print_error(int count, const char *desc);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset);
void windowSizeCallback(GLFWwindow *window, int xoffset, int yoffset);

int WIDTH = 1280;
int HEIGHT = 720;
float ASPECT_RATIO = WIDTH / (float)HEIGHT;
// constants of the program

float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
Camera camera = Camera(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), glm::radians(60.0f), ASPECT_RATIO, 0.1f, 1000.0f);

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
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	glewExperimental = GL_TRUE;
	glewInit();
	Model model = Model("assets/iron_man/Iron_Man.obj");
	bool lightEnabled = true;
	model.SetLightActive(lightEnabled);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		model.Draw(camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(pitch, yaw, roll), 1.0f);
		model.Draw(camera, glm::vec3(-2.0f, -1.0f, -5.0f), glm::vec3(pitch, yaw, roll), 1.0f);
		model.Draw(camera, glm::vec3(2.0f, -1.0f, -5.0f), glm::vec3(pitch, yaw, roll), 1.0f);
		glfwSwapBuffers(window);
		glfwPollEvents();
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
	camera.position += glm::vec3(0.0f, 0.0f, -yoffset);
	camera.UpdateCamera();
	// std::cout << camera.position.x << "," << camera.position.y << "," << camera.position.z << std::endl;
}

void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	// pitch = -yoffset / HEIGHT * 360;
	yaw = xoffset / WIDTH * 360;
}

void windowSizeCallback(GLFWwindow *window, int xoffset, int yoffset)
{
	WIDTH = xoffset;
	HEIGHT = yoffset;
	ASPECT_RATIO = WIDTH / HEIGHT;
	glViewport(0, 0, WIDTH, HEIGHT);
}