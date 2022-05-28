#include "../header/Model.h"
#include <fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../header/stb_image.h"

using namespace RenderEngine;

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
    std::string line;
    std::ifstream file(obj_model_filepath);
    std::string homeDirectiory = "";
    std::string objMTL;
    bool readNormals = false;
    bool readTextureCoord = false;
    // Load vertices/Normals/Texture Coordinates
    if (file.is_open())
    {
        std::cout << "File Open: " << obj_model_filepath << std::endl;
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
                x = std::stod(words[1]);
                y = std::stod(words[2]);
                z = std::stod(words[3]);
                glm::vec3 vert = glm::vec3(x, y, z);
                vertices.push_back(vert);
            }
            else if (strcmp(words[0].c_str(), "vt") == 0)
            {
                // texture coordinate
                readTextureCoord = true;
                x = std::stod(words[1]);
                y = std::stod(words[2]);
                glm::vec2 coord = glm::vec2(x, y);
                texCoord.push_back(coord);
            }
            else if (strcmp(words[0].c_str(), "vn") == 0)
            {
                // normal vector
                readNormals = true;
                x = std::stod(words[1]);
                y = std::stod(words[2]);
                z = std::stod(words[3]);
                glm::vec3 normal = glm::vec3(x, y, z);
                normalVertices.push_back(normal);
            }
            else if (strcmp(words[0].c_str(), "f") == 0)
            {
                int v[3] = {0, 0, 0};
                int vt[3] = {-1, -1, -1};
                int vn[3] = {-1, -1, -1};
                std::vector<unsigned int> newFaces = std::vector<unsigned int>(0);
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
                    newFaces.push_back(face.x);
                    newFaces.push_back(face.y);
                    newFaces.push_back(face.z);
                }
                faces.push_back(newFaces);
            }
        }
        file.close();
    }
    else
    {
        throw std::runtime_error("Unable to open OBj file!");
    }
    file.close();
    std::vector<std::string> objFilepathFolders = GetElementsOfLine(obj_model_filepath, '/');
    objFilepathFolders.erase(objFilepathFolders.end());
    for (int i = 0; i < objFilepathFolders.size(); i++)
    {
        homeDirectiory += objFilepathFolders[i] + "/";
    }
    std::string home = homeDirectiory;
    homeDirectiory += objMTL;
    homeDirectiory.at(homeDirectiory.length() - 1) = '\0';

    std::ifstream mtlFile(homeDirectiory);
    if (mtlFile.is_open())
    {
        std::cout << "File Open: " << homeDirectiory << std::endl;
        while (getline(mtlFile, line))
        {
            std::vector<std::string> words = GetElementsOfLine(line, ' ');

            if (strcmp(words[0].c_str(), "Ns") == 0)
            {
                Ns = std::stof(words[1]);
            }
            else if (strcmp(words[0].c_str(), "Ka") == 0)
            {
                Ka = glm::vec3(stof(words[1]), stof(words[2]), stof(words[3]));
            }
            else if (strcmp(words[0].c_str(), "Kd") == 0)
            {
                Kd = glm::vec3(stof(words[1]), stof(words[2]), stof(words[3]));
            }
            else if (strcmp(words[0].c_str(), "Ks") == 0)
            {
                Ks = glm::vec3(stof(words[1]), stof(words[2]), stof(words[3]));
            }
            else if (strcmp(words[0].c_str(), "map_Kd") == 0)
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

    std::cout << "Data: " << std::endl;
    std::cout << "Model MTL: " << objMTL << std::endl;
    std::cout << "Vertices: " << vertices.size() << std::endl;
    std::cout << "Texture coordinates: " << texCoord.size() << std::endl;
    std::cout << "Normal Vectors: " << normalVertices.size() << std::endl;
    std::cout << "Model Faces: " << faces.size() << std::endl;
}

void Model::Send(void)
{
    // generate vertex array object
    glGenVertexArrays(1, &vertexArrayObject);
    // bind the vertex array object of the model
    glBindVertexArray(vertexArrayObject);
    // generate Vertex buffer object
    glGenBuffers(2, vertexBufferObject);
    std::cout<<"Create Vertex array object"<<std::endl;
    
    //activate current buffer object
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[0]);
    //Fetch vertices stored in the class and make an array from it
    glm::vec3 modelVertices[vertices.size() * 3];
    std::copy(vertices.begin(), vertices.end(), modelVertices);
    //bind data to the current active buffer object
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), modelVertices, GL_STATIC_DRAW);
    //makes sure buffer has 3 float numbers per vertex
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    //Start counting the vertices at the beginning of the array
    glEnableVertexAttribArray(0);
    std::cout<<"Create Vertex buffer object"<<std::endl;
}

void Model::Draw(glm::vec3 position, glm::vec3 orientation)
{
}