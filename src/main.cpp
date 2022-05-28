#include <iostream>
#include <vector>
#include <string.h>

#include <time.h>
// File manipulation lib
// #include <fstream>

#include "../header/Model.h"
using namespace RenderEngine;

// callback functions
void print_error(int count, const char *desc);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset);

// constants of the program
const int WIDTH = 800;
const int HEIGHT = 600;
const float ASPECT_RATIO = WIDTH / (float)HEIGHT;

// values for testing models
float zoom = -10.0f;
float pitch = 0;
float yaw = 0;

int main()
{
    srand(time(0));
    GLFWwindow *window = nullptr;
    glfwSetErrorCallback(print_error);

    if (!glfwInit())
        return 1;

    window = glfwCreateWindow(WIDTH, HEIGHT, "My First P3D Project", NULL, NULL);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();
    Model model = Model("assets/iron_man/Iron_Man.obj");

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 view = glm::lookAt(
            glm::vec3(0, 100, zoom),
            glm::vec3(0, 100, 1),
            glm::vec3(0, 1, 0));
        glm::mat4 model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(pitch), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(yaw), glm::vec3(0, 1, 0));

        // MVP = camera.getCameraMatrix() * model;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
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