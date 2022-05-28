#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>

#include "../header/Engine.h"
#include "../header/LoadShaders.h"

static std::vector<std::string> GetElementsOfLine(const std::string line, const char element);

namespace Graphics
{
    class RenderEngine
    {
    private:
        int numVertexArrayObjects; // 1
        int numBuffers;            // vertices, colors, EBO
        GLuint *vertexArrayObjects;
        GLuint *buffers;
        std::vector<Engine::Model> importedModels;

    public:
        int numberOfModels;

        RenderEngine();
        ~RenderEngine();

        /*
        Read data from model file(.obj)
        */
        void Read(const std::string obj_model_filepath);
        /*
        Send data stored in the object to the Graphics Card
        */
        void Send(void);
        /*
        Draw data stored in Graphics Card
        */
        void Draw(glm::vec3 position, glm::vec3 orientation);

        void print_gl_info();
        void print_error(int error, const char *description);
        void init(void);
    };

    class Camera
    {
    private:
        // default values
        glm::vec3 position;  //= glm::vec3(0, 0, 0);
        glm::vec3 direction; // = glm::vec3(0, 0, 1);
        glm::vec3 up;        // = glm::vec3(0, 1, 0);
        float nearPlane;     // = 0.1f;
        float farPlane;      // = 1000.0f;
        float fieldOfView;   // = 60.0f;
        float aspect;        // = 16.0f / 9.0f;

        glm::mat4 view;            // = glm::lookAt(position, position + glm::vec3(0, 0, 1), up);
        glm::mat4 projection;      // = glm::perspective(fieldOfView, aspect, nearPlane, farPlane);
        glm::mat4 view_projection; // = projection * view;

    public:
        Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float fieldOfView, float nearPlane, float farPlane, float aspect);
        Camera();
        ~Camera();

        glm::mat4 getCameraMatrix(void);
        glm::vec3 getCameraPosition(void);
        glm::vec3 getCameraDirection(void);

        void setCameraDirection(glm::vec3 direction);
        void setCameraPosition(glm::vec3 position);
        void updateCamera(void);
        void countFPS(void);
    };
}