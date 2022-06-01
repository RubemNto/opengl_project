#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
using namespace std;

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>

#include "../header/Camera.h"

namespace RenderEngine
{
    class Model
    {
    private:
        struct Material
        {
            float Ns;
            glm::vec3 Ka, Kd, Ks;
            std::string imgName;
            std::string imagePath;
            int imgWidth, imgHeight, imgNumChannels;
        };
        // Read Data and only store the data in arrays and draw arrays

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textureCoordinates;

        GLuint vertexArrayObject;
        GLuint vertexBufferObjects[3];
        GLuint shaderProgram;

        GLint lightEnabled;
        std::map<std::string, Material> materials;
        /*Read data from OBJ file*/
        void ReadModel(const std::string file_name);
        /*Read MTl file from OBJ file*/
        void ReadMaterial(const std::string file_name);
        /*Send data read from OBJ file to Graphics Card*/
        void SendModelData(void);
        void LoadTexture(const std::string file_name);

        std::vector<std::string> GetElementsOfLine(const std::string line, const char element);

    public:
        Model(const std::string obj_model_filepath);
        ~Model();
        void PrintModelData();
        void SetLightActive(bool value);
        void Draw(Camera camera, glm::vec3 position, glm::vec3 orientation, float scale);
    };
}