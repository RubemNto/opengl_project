#include "../header/Model.h"
#include <fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../header/stb_image.h"

using namespace RenderEngine;

// TODO: make function return matrix of array of char
static std::vector<std::string> GetElementsOfLine(const std::string line, const char element) 
{
    std::vector<std::string> elements;
    std::string word;
    for (int i = 0; i < line.length(); i++)
    {
        char l = line[i];
        if (l == element)
        {
            if (strcmp(word.c_str(), "") != 0)
            {
                elements.push_back(word);
                word = "";
            }
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

Model::Model(const std::string obj_model_filepath)
{
    Read(obj_model_filepath);
    Send();
}

Model::~Model() {}

void Model::Read(const std::string obj_model_filepath)
{
    std::vector<GLfloat> vtx;
    std::vector<GLfloat> texCoord;
    std::vector<GLfloat> normVtx;
    std::vector<GLuint> fElements;

    std::string line;
    std::ifstream file(obj_model_filepath);
    std::string objMTL;
    bool readNormals = false;
    bool readTextureCoord = false;
    if (file.is_open())
    {
        std::cout << "File Open: " << obj_model_filepath << std::endl;
        while (getline(file, line))
        {
            float x, y, z;
            std::vector<std::string> words = GetElementsOfLine(line, ' ');
            if (strcmp(words[0].c_str(), "mtllib") == 0) // Get the mtl file name to be read in the future
            {
                objMTL = words[1];
            }
            else if (strcmp(words[0].c_str(), "v") == 0) // Get vertex data
            {
                x = std::stod(words[1]);
                y = std::stod(words[2]);
                z = std::stod(words[3]);
                vtx.push_back(x);
                vtx.push_back(y);
                vtx.push_back(z);
            }
            else if (strcmp(words[0].c_str(), "vt") == 0) // Get Texture Coordinate
            {
                readTextureCoord = true;
                x = std::stod(words[1]);
                y = std::stod(words[2]);
                texCoord.push_back(x);
                texCoord.push_back(y);
            }
            else if (strcmp(words[0].c_str(), "vn") == 0) // Get Vertex Normal
            {
                // normal vector
                readNormals = true;
                x = std::stod(words[1]);
                y = std::stod(words[2]);
                z = std::stod(words[3]);
                normVtx.push_back(x);
                normVtx.push_back(y);
                normVtx.push_back(z);
            }
            else if (strcmp(words[0].c_str(), "f") == 0) // Get faces elements
            {
                for (int i = 0; i < 3; i++)
                {
                    std::vector<std::string> faceData = GetElementsOfLine(words[i + 1], '/');
                    unsigned int v = -1, vt = -1, vn = -1;
                    v = std::stoi(faceData[0]) - 1;
                    if (readTextureCoord == true) // check for the existence of texture coordinate
                    {
                        vt = std::stoi(faceData[1]) - 1;
                    }
                    if (readNormals == true) // check for the existence of vertex normal
                    {
                        vn = std::stoi(faceData[2]) - 1;
                    }
                    fElements.push_back(v);
                    fElements.push_back(vt);
                    fElements.push_back(vn);
                }
            }
        }
        file.close();
    }
    else
    {
        throw std::runtime_error("Unable to open OBj file!");
    }
    file.close();
    // Get Home Directory Folders
    std::string homeDirectiory = "";
    std::vector<std::string> objFilepathFolders = GetElementsOfLine(obj_model_filepath, '/');
    objFilepathFolders.erase(objFilepathFolders.end());
    for (int i = 0; i < objFilepathFolders.size(); i++)
    {
        homeDirectiory += objFilepathFolders[i] + "/";
    }
    // Set home directory of the file
    std::string home = homeDirectiory;
    // Get objMTL file
    homeDirectiory += objMTL;
    homeDirectiory.at(homeDirectiory.length() - 1) = '\0';

    std::ifstream mtlFile(homeDirectiory);
    if (mtlFile.is_open())
    {
        std::cout << "File Open: " << homeDirectiory << std::endl;
        while (getline(mtlFile, line))
        {
            std::vector<std::string> words = GetElementsOfLine(line, ' ');

            if (strcmp(words[0].c_str(), "Ns") == 0)// Get Specular value
            {
                Ns = std::stof(words[1]);
            }
            else if (strcmp(words[0].c_str(), "Ka") == 0)// Get Ambient Color
            {
                Ka = glm::vec3(stof(words[1]), stof(words[2]), stof(words[3]));
            }
            else if (strcmp(words[0].c_str(), "Kd") == 0)// Get Diffuse Color
            {
                Kd = glm::vec3(stof(words[1]), stof(words[2]), stof(words[3]));
            }
            else if (strcmp(words[0].c_str(), "Ks") == 0)// Get Specular Color
            {
                Ks = glm::vec3(stof(words[1]), stof(words[2]), stof(words[3]));
            }
            else if (strcmp(words[0].c_str(), "map_Kd") == 0)// Get Texture Map
            {
                std::string imgPath = home + words[1];
                imgPath.pop_back();
                std::cout << "Texture Path: " << imgPath.c_str() << std::endl;
                imgData = stbi_load(imgPath.c_str(), &imgWidth, &imgHeight, &imgNumChannels, 0);
            }
        }
    }
    else
    {
        throw std::runtime_error("Unable to open MTL file!");
    }
    mtlFile.close();

    vertices = new GLfloat[vtx.size()];
    std::copy(vtx.begin(), vtx.end(), vertices);
    textureCoordinates = new GLfloat[texCoord.size()];
    std::copy(texCoord.begin(), texCoord.end(), textureCoordinates);
    vertexNormals = new GLfloat[normVtx.size()];
    std::copy(normVtx.begin(), normVtx.end(), vertexNormals);
    faceElements = new GLuint[fElements.size()];
    std::copy(fElements.begin(), fElements.end(), faceElements);

    std::cout << "Model Data " << std::endl;
    std::cout << "\tModel MTL: " << objMTL << std::endl;
    std::cout << "\tVertices: " << (vtx.size()) / 3 << std::endl;
    std::cout << "\tTexture coordinates: " << (texCoord.size()) / 2 << std::endl;
    std::cout << "\tNormal Vectors: " << (normVtx.size()) / 3 << std::endl;
    std::cout << "\tModel Faces: " << (fElements.size()) / 9 << std::endl;
}

void Model::Send(void)
{
    // // generate vertex array object
    // glGenVertexArrays(1, &vertexArrayObject);
    // // bind the vertex array object of the model
    // glBindVertexArray(vertexArrayObject);
    // // generate Vertex buffer object
    // glGenBuffers(2, vertexBufferObject);
    // std::cout << "Create Vertex array object" << std::endl;

    // // activate current buffer object
    // glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[0]);
    // // Fetch vertices stored in the class and make an array from it
    // glm::vec3 modelVertices[vertices.size() * 3];
    // std::copy(vertices.begin(), vertices.end(), modelVertices);
    // // bind data to the current active buffer object
    // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), modelVertices, GL_STATIC_DRAW);
    // // makes sure buffer has 3 float numbers per vertex
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // // Start counting the vertices at the beginning of the array
    // glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[1]);
    // GLuint vertexElements[facesElements.size()];
    // std::copy(vertices.begin(), vertices.end(), modelVertices);
}

void Model::Draw(glm::vec3 position, glm::vec3 orientation)
{
}