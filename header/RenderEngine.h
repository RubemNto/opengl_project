#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace Graphics
{
    class RenderEngine
    {
    private:
    public:
        /*
        Read data from model file(.obj)
        */
        void Read(const std::string obj_model_filepath);
        /*
        Send data from Read function to Graphics Card
        */
        void Send(void);
        /*
        Draw data stored in Graphics Card
        */
        void Draw(glm::vec3 position, glm::vec3 orientation);
        RenderEngine();
        ~RenderEngine();
    };

    class Camera
    {
    private:
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 direction = glm::vec3(0, 0, 1);
        glm::vec3 up = glm::vec3(0, 1, 0);
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        float fieldOfView = 60.0f;
        float aspect = 16.0f / 9.0f;

        glm::mat4 view = glm::lookAt(position, position + glm::vec3(0, 0, 1), up);
        glm::mat4 projection = glm::perspective(fieldOfView, aspect, nearPlane, farPlane);
        glm::mat4 view_projection = projection * view;

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