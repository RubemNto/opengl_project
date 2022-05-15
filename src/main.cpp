#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

// callback functions
void print_error(int count, const char *desc);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset);
void Draw(glm::mat4 mvp, std::vector<glm::vec3> obj);

// testing library funtions
std::vector<glm::vec3> LoadCube();

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

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), ASPECT_RATIO, 0.1f, 100.0f);

    glClearColor(0, 0, 0, 1);
    std::vector<glm::vec3> cube = LoadCube();
    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 view = glm::lookAt(
            glm::vec3(0, 0, zoom),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 1, 0));
        glm::mat4 model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(pitch), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(yaw), glm::vec3(0, 1, 0));

        glm::mat4 MVP = projection * view * model;

        Draw(MVP, cube);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    return 0;
}

//TEsting version of Draw to test MVP matrix and loading simple models
void Draw(glm::mat4 mvp, std::vector<glm::vec3> obj)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float *vertex_stream = static_cast<float *>(glm::value_ptr(obj.front()));

    std::vector<glm::vec3> colors{
        glm::vec3(1.0f, 0.0f, 0.0f), // Red
        glm::vec3(1.0f, 1.0f, 0.0f), // Yellow
        glm::vec3(0.0f, 1.0f, 0.0f), // Green
        glm::vec3(0.0f, 1.0f, 1.0f), // Cyan
        glm::vec3(0.0f, 0.0f, 1.0f), // Blue
        glm::vec3(1.0f, 0.0f, 1.0f)  // Magenta
    };

    // Desenha quad em modo imediato
    glBegin(GL_QUADS);
    for (int nv = 0; nv < 6 * 4 * 3; nv += 3)
    {
        glColor3f(colors[nv / (4 * 3)].r, colors[nv / (4 * 3)].g, colors[nv / (4 * 3)].b);
        glm::vec4 vertex = glm::vec4(vertex_stream[nv], vertex_stream[nv + 1], vertex_stream[nv + 2], 1.0f);
        glm::vec4 transformed_vertex = mvp * vertex;
        glm::vec4 normalized_vertex = transformed_vertex / transformed_vertex.w;
        glVertex3f(normalized_vertex.x, normalized_vertex.y, normalized_vertex.z);
    }
    glEnd();
}

void print_error(int count, const char *desc)
{
    std::cout << desc << std::endl;
}
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    zoom -= yoffset;
    std::cout << "Zoom: " << zoom << std::endl;
}

void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    pitch = yoffset;
    yaw = xoffset;
}

std::vector<glm::vec3> LoadCube()
{
    glm::vec3 point[24] = {

        glm::vec3(-1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(-1.0f, 1.0f, 1.0f),

        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, 1.0f, -1.0f),
        glm::vec3(1.0f, 1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),

        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, 1.0f, -1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),

        glm::vec3(-1.0f, -1.0f, 1.0f),
        glm::vec3(-1.0f, 1.0f, 1.0f),
        glm::vec3(-1.0f, 1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),

        glm::vec3(-1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, -1.0f),
        glm::vec3(-1.0f, 1.0f, -1.0f),

        glm::vec3(-1.0f, -1.0f, 1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, 1.0f)};

    std::vector<glm::vec3> ret;
    for (auto i : point)
        ret.push_back(i);

    return ret;
}