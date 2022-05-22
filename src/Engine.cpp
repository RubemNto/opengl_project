#include "../header/Engine.h"
// #include "../header/RenderEngine.h"

// using namespace Graphics;

namespace Engine
{
    Model::Model(const std::string modelPath)
    {
    }

    Model::~Model(){};

    void Model::setVertices(std::vector<glm::vec3> data)
    {
        vertices = data;
    }

    void Model::setVertexNormals(std::vector<glm::vec3> data)
    {
        vertexNormals = data;
    }

    void Model::setTextureCoordinates(std::vector<glm::vec2> data)
    {
        textureCoordinates = data;
    }

    void Model::setFaces(std::vector<std::vector<GLuint>> data)
    {
        faces = data;
    }

    std::vector<glm::vec3> Model::getVertices()
    {
        return vertices;
    }
    std::vector<glm::vec2> Model::getTextureCoordinates()
    {
        return textureCoordinates;
    }
    std::vector<glm::vec3> Model::getVertexNormals()
    {
        return vertexNormals;
    }

    std::vector<std::vector<GLuint>> Model::getFaces()
    {
        return faces;
    }

    void Model::setAmbientLightReflectionCoefitient(glm::vec3 data) 
    {
        ambientLightReflectionCoefitient = data;
    }
    void Model::setDiffuseLightReflectionCoefitient(glm::vec3 data) 
    {
        diffuseLightReflectionCoefitient = data;
    }
    void Model::setSpecularLightReflectionCoefitient(glm::vec3 data) 
    {
        specularLightReflectionCoefitient = data;
    }
    void Model::setSpecularExponent(float data) 
    {
        specularExponent = data;
    }

}