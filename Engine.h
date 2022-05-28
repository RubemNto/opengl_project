#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <string>
// File manipulation lib
#include <fstream>

namespace Engine
{
    class Model
    {
    private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<glm::vec3> vertexNormals;
        /*List of vectors with the indexes (Vertex/Texture Coordinate/Vertex Normal) per vertex for each face*/
        std::vector<std::vector<GLuint>> faces;

        glm::vec3 ambientLightReflectionCoefitient;
        glm::vec3 diffuseLightReflectionCoefitient;
        glm::vec3 specularLightReflectionCoefitient;
        float specularExponent;


    public:
        std::vector<std::string> maps;
        
        Model(const std::string modelPath);
        ~Model();
        std::vector<glm::vec3> getVertices();
        std::vector<glm::vec2> getTextureCoordinates();
        std::vector<glm::vec3> getVertexNormals();
        std::vector<std::vector<GLuint>> getFaces();

        void setVertices(std::vector<glm::vec3> data);
        void setTextureCoordinates(std::vector<glm::vec2> data);
        void setVertexNormals(std::vector<glm::vec3> data);
        void setFaces(std::vector<std::vector<GLuint>> data);

        void setAmbientLightReflectionCoefitient(glm::vec3 data);
        void setDiffuseLightReflectionCoefitient(glm::vec3 data);
        void setSpecularLightReflectionCoefitient(glm::vec3 data);
        void setSpecularExponent(float data);
    };
}