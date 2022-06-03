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
	model.AddLight(
		new AmbientLight(
			true,
			1,
			glm::vec3(1, 1, 1)));
	model.AddLight(
		new DirectionalLight(
			glm::vec3(0.0f, 1.0f, 0.0f),
			true,
			1,
			glm::vec3(1, 1, 1)));
	model.AddLight(
		new PointLight(
			glm::vec3(-5, 1, -2.5),
			true,
			1,
			glm::vec3(1.0f, 0.0f, 0.0f)));
	model.AddLight(
		new SpotLight(
			glm::vec3(5, 1, 2.5),
			glm::vec3(0, 1, 0),
			glm::radians(60.0f),
			true,
			1,
			glm::vec3(1, 1, 1)));

	model.SetLightActive(true);
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

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		for (int i = 0; i < model.numLights; i++)
		{
			Light *light = model.GetLight(i);
			if (light->type == LightType::DIRECTIONAL_LIGHT)
			{
				AmbientLight *dLight = (AmbientLight *)light;
				std::cout << "Ambient Light Data" << std::endl;
				std::cout << "\tLight Color:" << dLight->color.x << "," << dLight->color.y << "," << dLight->color.z << std::endl;
				std::cout << "\tLight Power:" << dLight->power << std::endl;
				std::cout << "\tLight State:" << dLight->active << std::endl;
			}
		}
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		for (int i = 0; i < model.numLights; i++)
		{
			Light *light = model.GetLight(i);
			if (light->type == LightType::DIRECTIONAL_LIGHT)
			{
				DirectionalLight *dLight = (DirectionalLight *)light;
				std::cout << "Directional Light Data" << std::endl;
				std::cout << "\tLight Color:" << dLight->color.x << "," << dLight->color.y << "," << dLight->color.z << std::endl;
				std::cout << "\tLight Power:" << dLight->power << std::endl;
				std::cout << "\tLight State:" << dLight->active << std::endl;
				std::cout << "\tLight Direction:" << dLight->orientation.x << "," << dLight->orientation.y << "," << dLight->orientation.z << std::endl;
			}
		}
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		for (int i = 0; i < model.numLights; i++)
		{
			Light *light = model.GetLight(i);
			if (light->type == LightType::POINT_LIGHT)
			{
				PointLight *pLight = (PointLight *)light;
				std::cout << "Point Light Data" << std::endl;
				std::cout << "\tLight Color:" << pLight->color.x << "," << pLight->color.y << "," << pLight->color.z << std::endl;
				std::cout << "\tLight Power:" << pLight->power << std::endl;
				std::cout << "\tLight State:" << pLight->active << std::endl;
				std::cout << "\tLight Position:" << pLight->position.x << "," << pLight->position.y << "," << pLight->position.z << std::endl;
			}
		}
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		for (int i = 0; i < model.numLights; i++)
		{
			Light *light = model.GetLight(i);
			if (light->type == LightType::SPOT_LIGHT)
			{
				SpotLight *sLight = (SpotLight *)light;
				std::cout << "Spot Light Data" << std::endl;
				std::cout << "\tLight Color:" << sLight->color.x << "," << sLight->color.y << "," << sLight->color.z << std::endl;
				std::cout << "\tLight Power:" << sLight->power << std::endl;
				std::cout << "\tLight State:" << sLight->active << std::endl;
				std::cout << "\tLight Position:" << sLight->position.x << "," << sLight->position.y << "," << sLight->position.z << std::endl;
				std::cout << "\tLight Orientation:" << sLight->orientation.x << "," << sLight->orientation.y << "," << sLight->orientation.z << std::endl;
				std::cout << "\tLight CutoffAngle:" << sLight->cutoffAngle << std::endl;
			}
		}
	}
}