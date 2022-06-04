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
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
int WIDTH = 1280;
int HEIGHT = 720;
float ASPECT_RATIO = WIDTH / (float)HEIGHT;
// constants of the program

float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
Camera camera = Camera(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), glm::radians(60.0f), ASPECT_RATIO, 0.1f, 1000.0f);
Model model;
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
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();
	model = Model("assets/iron_man/Iron_Man.obj");
	model.ambientLight = new AmbientLight(false, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	model.directionalLight = new DirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), false, 1.0f, glm::vec3(0, 0, 0));
	model.pointLight = new PointLight(glm::vec3(0, 2, 2.5f), false, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	model.spotLight = new SpotLight(camera.position, camera.orientation, glm::radians(60.0f), false, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	float lightDirAngle = 0;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		model.directionalLight->orientation = glm::vec3(glm::cos(glm::radians(lightDirAngle)), 1, glm::sin(glm::radians(lightDirAngle)));
		model.pointLight->position = glm::vec3(glm::sin(glm::radians(lightDirAngle)), 0.0f, glm::sin(glm::radians(lightDirAngle))) * 5.0f;
		model.spotLight->orientation = glm::vec3(glm::cos(glm::radians(lightDirAngle)), glm::sin(glm::radians(lightDirAngle)), -1);
		model.Draw(camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(pitch, yaw, roll), 1.0f);
		model.Draw(camera, glm::vec3(-2.0f, -1.0f, -5.0f), glm::vec3(pitch, yaw, roll), 1.0f);
		model.Draw(camera, glm::vec3(2.0f, -1.0f, -5.0f), glm::vec3(pitch, yaw, roll), 1.0f);

		lightDirAngle += 1.0f;

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
}

void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	yaw = xoffset / WIDTH * 360;
}

void windowSizeCallback(GLFWwindow *window, int xoffset, int yoffset)
{
	WIDTH = xoffset;
	HEIGHT = yoffset;
	ASPECT_RATIO = WIDTH / HEIGHT;
	glViewport(0, 0, WIDTH, HEIGHT);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		std::cout << "Ambient Light State: " << std::endl;
		model.ambientLight->active = !model.ambientLight->active;
		std::cout << model.ambientLight->active << std::endl;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		std::cout << "Directional Light State: " << std::endl;
		model.directionalLight->active = !model.directionalLight->active;
		std::cout << model.directionalLight->active << std::endl;
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		std::cout << "Point Light State: " << std::endl;
		model.pointLight->active = !model.pointLight->active;
		std::cout << model.pointLight->active << std::endl;
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		std::cout << "Spot Light State: " << std::endl;
		model.spotLight->active = !model.spotLight->active;
		std::cout << model.spotLight->active << std::endl;
	}

	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		std::cout << "Deform State: " << std::endl;
		model.deform = !model.deform;
		std::cout << model.deform << std::endl;
	}
}