#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string.h>

// File manipulation lib
#include <fstream>

// callback functions
void print_error(int count, const char *desc);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset);

void Read(const std::string obj_model_filepath);
// void Send(void);
// void Draw(glm::vec3 position, glm::vec3 orientation);

// testing library funtions
// std::vector<glm::vec3> LoadCube();

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

    // // Read Model
    Read("assets/Iron_Man.obj");

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), ASPECT_RATIO, 0.1f, 100.0f);

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
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
    zoom -= yoffset;
    std::cout << "Zoom: " << zoom << std::endl;
}

void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    pitch = -yoffset / HEIGHT * 360;
    yaw = -xoffset / WIDTH * 360;
}

std::vector<std::string> GetElementsOfLine(const std::string line, const char element)
{
    std::vector<std::string> elements = std::vector<std::string>(0);
    std::string word;
    for (int i = 0; i < line.length(); i++)
    {
        char l = line[i];
        if (l == element) // || i == line.length() - 1)
        {
            elements.push_back(word);
            word = "";
        }
        else
        {
            word += l;
        }
    }
    if (strcmp(word.c_str(), "") != 0)
    {
        elements.push_back(word);
        word = "";
    }
    return elements;
}

// Only read Obj files
void Read(const std::string obj_model_filepath)
{
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>(0);
    std::vector<glm::vec2> texCoord = std::vector<glm::vec2>(0);
    std::vector<glm::vec3> normalVertices = std::vector<glm::vec3>(0);
    std::vector<std::vector<glm::vec3>> faces = std::vector<std::vector<glm::vec3>>(0);

    std::string line;
    std::ifstream file(obj_model_filepath);

    std::string objMTL;
    bool readNormals = false;
    bool readTextureCoord = false;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            float x, y, z;
            std::vector<std::string> words = GetElementsOfLine(line, ' ');
            if (strcmp(words[0].c_str(), "mtllib") == 0)
            {
                objMTL = words[1];
            }
            else if (strcmp(words[0].c_str(), "v") == 0)
            {
                // vertice
                x = std::stof(words[1]);
                y = std::stof(words[2]);
                z = std::stof(words[3]);
                glm::vec3 vert = glm::vec3(x, y, z);
                vertices.push_back(vert);
            }
            else if (strcmp(words[0].c_str(), "vt") == 0)
            {
                // texture coordinate
                readTextureCoord = true;
                x = std::stof(words[1]);
                y = std::stof(words[2]);
                glm::vec2 coord = glm::vec2(x, y);
                texCoord.push_back(coord);
            }
            else if (strcmp(words[0].c_str(), "vn") == 0)
            {
                // normal vector
                readNormals = true;
                x = std::stof(words[1]);
                y = std::stof(words[2]);
                z = std::stof(words[3]);
                glm::vec3 normal = glm::vec3(x, y, z);
                normalVertices.push_back(normal);
            }
            else if (strcmp(words[0].c_str(), "f") == 0)
            {
                int v[3] = {0};
                int vt[3] = {-1};
                int vn[3] = {-1};
                std::vector<glm::vec3> newFaces = std::vector<glm::vec3>(0);
                for (int i = 0; i < 3; i++)
                {
                    std::vector<std::string> faceData = GetElementsOfLine(words[i + 1], '/');
                    v[i] = std::stoi(faceData[0]) - 1;
                    if (readTextureCoord == true)
                    {
                        vt[i] = std::stoi(faceData[1]) - 1;
                    }
                    if (readNormals == true)
                    {
                        vn[i] = std::stoi(faceData[2]) - 1;
                    }
                    glm::vec3 face = glm::vec3(v[i], vt[i], vn[i]);
                    newFaces.push_back(face);
                }
                faces.push_back(newFaces);
            }
        }
        file.close();
    }
    else
    {
        std::
                cout
            << "Unable to open file" << std::endl;
    }
    std::cout << "Data: " << std::endl;
    std::cout << "Vertices MTL: " << objMTL << std::endl;
    std::cout << "Vertices: " << vertices.size() << std::endl;
    std::cout << "Texture coordinates: " << texCoord.size() << std::endl;
    std::cout << "Normal Vectors: " << normalVertices.size() << std::endl;
    std::cout << "Mesh Faces: " << faces.size() << std::endl;
}
