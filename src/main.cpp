#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string.h>

#include <time.h>

// File manipulation lib
#include <fstream>

// callback functions
void print_error(int count, const char *desc);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void cursorPositionCallback(GLFWwindow *window, double xoffset, double yoffset);

std::vector<glm::vec3> Read(const std::string obj_model_filepath);
// void Send(void);
void Draw(std::vector<glm::vec3> model, glm::vec3 position, glm::vec3 orientation);

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

glm::mat4 MVP;
glm::vec3 pos;
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

    // // Read Model
    std::vector<glm::vec3> modelData = Read("assets/Iron_Man.obj");

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), ASPECT_RATIO, 0.1f, 100.0f);

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glm::vec3 scale = glm::vec4(1.1f);

    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 view = glm::lookAt(
            glm::vec3(0, 0, zoom),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 1, 0));
        glm::mat4 model = glm::mat4(1);
        model = glm::scale(model, scale);
        model = glm::rotate(model, glm::radians(pitch), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(yaw), glm::vec3(0, 1, 0));
        model = glm::translate(model, pos);

        MVP = projection * view * model;
        Draw(modelData, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
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
    // pitch = -yoffset / HEIGHT * 360;
    // yaw = -xoffset / WIDTH * 360;
    yaw = 180;//-xoffset / WIDTH * 360;
    pos = glm::vec3(1 - 2 * (xoffset / WIDTH), 1 - 2 * (yoffset / HEIGHT), 0)*zoom;
    std::cout << pos.x << " " << pos.y << std::endl;
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

//test for debug Read function 
void Draw(std::vector<glm::vec3> model, glm::vec3 position, glm::vec3 orientation)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float *vertex_stream = static_cast<float *>(glm::value_ptr(model.front()));
    glBegin(GL_TRIANGLES);
    for (int nv = 0; nv < (model.size() / 3) * 3 * 3; nv += 3)
    {
        glColor3f(rand() % 2 / 10.0f, rand() % 2 / 10.0f, rand() % 2 / 10.0f); // colors[nv / (4 * 3)].r, colors[nv / (4 * 3)].g, colors[nv / (4 * 3)].b);
        glm::vec4 vertex = glm::vec4(vertex_stream[nv], vertex_stream[nv + 1], vertex_stream[nv + 2], 1.0f);
        glm::vec4 transformed_vertex = MVP * vertex;
        glm::vec4 normalized_vertex = transformed_vertex / transformed_vertex.w;
        glVertex3f(normalized_vertex.x, normalized_vertex.y, normalized_vertex.z);
    }
    glEnd();
}

// Only read Obj files
/*std::vector<*/ std::vector<glm::vec3> /*>*/ Read(const std::string obj_model_filepath)
{
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>(0);
    std::vector<glm::vec2> texCoord = std::vector<glm::vec2>(0);
    std::vector<glm::vec3> normalVertices = std::vector<glm::vec3>(0);
    std::vector<std::vector<glm::vec3>> faces = std::vector<std::vector<glm::vec3>>(0);

    std::vector<std::vector<glm::vec3>> data = std::vector<std::vector<glm::vec3>>(0);

    std::string line;
    std::ifstream file(obj_model_filepath);

    std::string objMTL;
    bool readNormals = false;
    bool readTextureCoord = false;

    // temp
    std::vector<glm::vec3> vert = std::vector<glm::vec3>(0);

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

        for (int i = 0; i < faces.size(); i++)
        {
            for (int b = 0; b < faces[i].size(); b++)
            {
                glm::vec3 v = vertices[faces[i][b].x];
                vert.push_back(v);
            }
        }

        // for (int i = 0; i < faces.size(); i++)
        // {
        //     for (int b = 0; b < faces[i].size(); b++)
        //     {
        //         std::vector<glm::vec3> tempData = std::vector<glm::vec3>(0);
        //         glm::vec3 vertice = vertices[faces[i][b].x];
        //         glm::vec3 textureCoord = glm::vec3(texCoord[faces[i][b].y], 0);
        //         glm::vec3 normal = normalVertices[faces[i][b].z];
        //         tempData.push_back(vertice);
        //         tempData.push_back(textureCoord);
        //         tempData.push_back(normal);
        //         data.push_back(tempData);
        //     }
        // }
    }
    else
    {
        throw std::runtime_error("Unable to open file!");
        // std::cout << "Unable to open file" << std::endl;
    }
    std::cout << "Data: " << std::endl;
    std::cout << "Vertices MTL: " << objMTL << std::endl;
    std::cout << "Vertices: " << vertices.size() << std::endl;
    std::cout << "Texture coordinates: " << texCoord.size() << std::endl;
    std::cout << "Normal Vectors: " << normalVertices.size() << std::endl;
    std::cout << "Mesh Faces: " << faces.size() << std::endl;
    return vert;
}
