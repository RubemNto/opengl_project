#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace RenderEngine
{
    class Model
    {
    private:
        float Ns;
        glm::vec3 Ka, Kd, Ks;
        unsigned char *imgData;
        int imgWidth, imgHeight, imgNumChannels;
        GLfloat *vertices;           // (vec3)
        GLfloat *textureCoordinates; // (vec2)
        GLfloat *vertexNormals;      // (vec3)
        GLuint *faceElements;        // (vertices / texCoord / normalVertices)

        GLuint vertexArrayObject;
        GLuint vertexBufferObject[6]; // vertices(3 float), elements(1 int), UV's(2f), UV elements(1 int), normal vectors(3f), normal vectors elements(1 int)

        void Read(const std::string obj_model_filepath);
        void Send(void);

    public:
        Model(const std::string obj_model_filepath);
        ~Model();

        void Draw(glm::vec3 position, glm::vec3 orientation);
    };
}