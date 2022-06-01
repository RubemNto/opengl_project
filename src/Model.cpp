#include "../header/Model.h"
// // #define STB_IMAGE_IMPLEMENTATION
// // #include "../header/stb_image.h"

using namespace RenderEngine;

Model::Model(const std::string obj_model_filepath)
{
    ReadModel(obj_model_filepath);
    // PrintModelData();
    SendModelData();
}

Model::~Model() {}

void Model::PrintModelData()
{
    std::cout << "Model Data" << std::endl;
    std::cout << "\tVertices: " << vertices.size() << std::endl;
    std::cout << "\tUV's: " << textureCoordinates.size() << std::endl;
    std::cout << "\tNormals: " << normals.size() << std::endl;
}

void Model::Draw(Camera camera, glm::vec3 position, glm::vec3 orientation, float scale)
{
    glm::mat4 Model = glm::mat4(1);
    Model = glm::scale(Model, glm::vec3(scale));

    Model = glm::rotate(Model, glm::radians(orientation.x), glm::vec3(1, 0, 0));
    Model = glm::rotate(Model, glm::radians(orientation.y), glm::vec3(0, 1, 0));
    Model = glm::rotate(Model, glm::radians(orientation.z), glm::vec3(0, 0, 1));

    Model = glm::translate(Model, position);

    glm::mat4 MVPMatrix = camera.GetViewProjectionMatrix() * Model;

    glBindVertexArray(vertexArrayObject);
    GLint mvp = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "MVP");
    glProgramUniformMatrix4fv(shaderProgram, mvp, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);
}

void Model::SendModelData(void)
{
    GLfloat vertices[this->vertices.size() * 3];
    GLfloat normals[this->normals.size() * 3];
    GLfloat textureCoordinates[this->textureCoordinates.size() * 2];

    for (int i = 0; i < this->vertices.size(); i++)
    {
        vertices[i * 3] = this->vertices.at(i).x;
        vertices[i * 3 + 1] = this->vertices.at(i).y;
        vertices[i * 3 + 2] = this->vertices.at(i).z;

        normals[i * 3] = this->normals.at(i).x;
        normals[i * 3 + 1] = this->normals.at(i).y;
        normals[i * 3 + 2] = this->normals.at(i).z;

        textureCoordinates[i * 2] = this->textureCoordinates.at(i).x;
        textureCoordinates[i * 2 + 1] = this->textureCoordinates.at(i).y;
    }

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    glGenBuffers(3, vertexBufferObjects);
    for (int i = 0; i < 3 - 1; i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[i]);
        if (i == 0)
        {
            glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);
        }
        else if (i == 1)
        {
            glBufferStorage(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, 0);
        }
        else
        {
            glBufferStorage(GL_ARRAY_BUFFER, sizeof(normals), normals, 0);
        }
    }

    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "Shader/VertexShader.vert"},
        {GL_FRAGMENT_SHADER, "Shader/FragmentShader.frag"},
        {GL_NONE, NULL}};
    shaderProgram = LoadShaders(shaders);
    if (!shaderProgram)
        exit(EXIT_FAILURE);
    glUseProgram(shaderProgram);

    GLint vertexPosition = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "vertexPosition");

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
    glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexPosition);

    // GLint mvp = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "MVP");
    // glProgramUniformMatrix4fv(shaderProgram, mvp, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FRONT_AND_BACK);
}

void Model::ReadModel(const std::string file_name)
{
    std::vector<std::string> folders = GetElementsOfLine(file_name, '/');
    std::string HomeFolder = folders.at(0) + "/" + folders.at(1) + "/";

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoordinates;

    std::ifstream file(file_name);
    if (file)
    {
        std::string currentMaterial;
        std::string line;
        std::vector<std::string> elements;
        while (std::getline(file, line))
        {
            elements = GetElementsOfLine(line, ' ');
            if (strcmp(elements.at(0).c_str(), "mtllib") == 0)
            {
                elements.at(1).pop_back();
                ReadMaterial(HomeFolder + elements.at(1));
            }
            if (strcmp(elements.at(0).c_str(), "v") == 0)
            {
                vertices.push_back(glm::vec3(
                    std::stof(elements.at(1)),
                    std::stof(elements.at(2)),
                    std::stof(elements.at(3))));
            }
            if (strcmp(elements.at(0).c_str(), "vt") == 0)
            {
                textureCoordinates.push_back(glm::vec2(
                    std::stof(elements.at(1)),
                    std::stof(elements.at(2))));
            }
            if (strcmp(elements.at(0).c_str(), "vn") == 0)
            {
                normals.push_back(glm::vec3(
                    std::stof(elements.at(1)),
                    std::stof(elements.at(2)),
                    std::stof(elements.at(3))));
            }
            if (strcmp(elements.at(0).c_str(), "f") == 0)
            {
                for (int i = 1; i < elements.size(); i++)
                {
                    std::vector<std::string> faceData = GetElementsOfLine(elements.at(i), '/');
                    int v, vt, vn;
                    v = std::stoi(faceData.at(0)) - 1;
                    vt = std::stoi(faceData.at(1)) - 1;
                    vn = std::stoi(faceData.at(2)) - 1;
                    this->vertices.push_back(vertices.at(v));
                    this->textureCoordinates.push_back(textureCoordinates.at(vt));
                    this->normals.push_back(normals.at(vn));
                }
            }
            elements.clear();
        }
    }
    else
    {
        std::cout << "Failed to load OBJ file" << std::endl;
    }
}

void Model::ReadMaterial(const std::string file_name)
{
    std::vector<std::string> folders = GetElementsOfLine(file_name, '/');
    std::string HomeFolder = folders.at(0) + "/" + folders.at(1) + "/";
    std::ifstream file(file_name);
    if (file)
    {
        std::string line;
        std::string MaterialName;
        while (std::getline(file, line))
        {
            std::vector<string> elements = GetElementsOfLine(line, ' ');
            if (strcmp(elements.at(0).c_str(), "newmtl") == 0)
            {
                MaterialName = elements.at(1);
                materials.insert({MaterialName, Material()});
            }
            if (strcmp(elements.at(0).c_str(), "Ns") == 0)
            {
                materials.at(MaterialName).Ns = std::stof(elements.at(1));
            }
            if (strcmp(elements.at(0).c_str(), "Ka") == 0)
            {
                materials.at(MaterialName).Ka = glm::vec3(std::stof(elements.at(1)), std::stof(elements.at(2)), std::stof(elements.at(3)));
            }
            if (strcmp(elements.at(0).c_str(), "Kd") == 0)
            {
                materials.at(MaterialName).Kd = glm::vec3(std::stof(elements.at(1)), std::stof(elements.at(2)), std::stof(elements.at(3)));
            }
            if (strcmp(elements.at(0).c_str(), "Ks") == 0)
            {
                materials.at(MaterialName).Ks = glm::vec3(std::stof(elements.at(1)), std::stof(elements.at(2)), std::stof(elements.at(3)));
            }
            if (strcmp(elements.at(0).c_str(), "map_Kd") == 0)
            {
                materials.at(MaterialName).imgName = elements.at(1);
                materials.at(MaterialName).imagePath = HomeFolder + elements.at(1);
            }
        }
    }
    else
    {
        std::cout << "Material file loading failed" << std::endl;
    }
}

std::vector<std::string> Model::GetElementsOfLine(const std::string line, const char element)
{
    std::vector<std::string> elements;
    std::string word;
    for (int i = 0; i < line.size(); i++)
    {
        char l = line.at(i);
        if (l != element && l != '\n')
        {
            word.push_back(l);
        }
        else
        {
            elements.push_back(word);
            word.clear();
        }
    }
    if (word.size() != 0)
        elements.push_back(word);
    return elements;
}

// bool Model::StartWith(std::string line, const char *text)
// {
//     int textLen = strlen(text);
//     if (line.size() < textLen)
//     {
//         return false;
//     }
//     for (int i = 0; i < textLen; i++)
//     {
//         if (line[i] == text[i])
//             continue;
//         else
//             return false;
//     }
//     return true;
// }

// void Model::Read(const std::string obj_model_filepath)
// {
//     std::vector<glm::vec3> vertex;
//     std::vector<glm::vec2> textureCoordinate;
//     std::vector<glm::vec3> vertexNormal;
//     std::vector<GLuint> faces;

//     std::string line;
//     std::string objMTL;

//     std::ifstream file(obj_model_filepath);
//     if (file.is_open())
//     {
//         std::cout << "File Open: " << obj_model_filepath << std::endl;
//         while (getline(file, line))
//         {
//             GLfloat x, y, z;
//             std::vector<std::string> words = GetElementsOfLine(line, ' ');
//             if (strcmp(words[0].c_str(), "mtllib") == 0) // Get the mtl file name to be read in the future
//             {
//                 objMTL = words[1];
//             }
//             else if (strcmp(words[0].c_str(), "v") == 0) // Get vertex data
//             {
//                 // Get String Data and turn into float
//                 x = std::stod(words[1]);
//                 y = std::stod(words[2]);
//                 z = std::stod(words[3]);
//                 vertex.push_back(glm::vec3(x, y, z));
//             }
//             else if (strcmp(words[0].c_str(), "vt") == 0) // Get Texture Coordinate
//             {
//                 // Get String Data and turn into float
//                 x = std::stod(words[1]);
//                 y = std::stod(words[3]);
//                 textureCoordinate.push_back(glm::vec2(x, y));
//             }
//             else if (strcmp(words[0].c_str(), "vn") == 0) // Get Vertex Normal
//             {
//                 // Get String Data and turn into float
//                 x = std::stod(words[1]);
//                 y = std::stod(words[2]);
//                 z = std::stod(words[3]);
//                 vertexNormal.push_back(glm::vec3(x, y, z));
//             }
//             else if (strcmp(words[0].c_str(), "f") == 0) // Get faces elements
//             {
//                 for (int i = 0; i < 3; i++)
//                 {
//                     std::vector<std::string> faceData = GetElementsOfLine(words[i + 1], '/');
//                     unsigned int v = 0, vt = 0, vn = 0;
//                     v = std::stoi(faceData[0]) - 1;
//                     vt = std::stoi(faceData[1]) - 1;
//                     vn = std::stoi(faceData[2]) - 1;
//                     faces.push_back(v);
//                     faces.push_back(vt);
//                     faces.push_back(vn);
//                 }
//             }
//         }
//         file.close();
//     }
//     else
//     {
//         throw std::runtime_error("Unable to open OBj file!");
//     }
//     file.close();

//     // Assign data to arrays in model class
//     vertices = (GLfloat *)malloc(sizeof(GLfloat) * 1); // only the first indexes of the list, with 3 flaot each(faces.size() / 3 * 3) = faces.size()
//     numVertices = faces.size();

//     textureCoordinates = new GLfloat[faces.size() / 3 * 2];
//     numTextureCoordinates = faces.size() / 3 * 2;

//     vertexNormals = new GLfloat[faces.size()];
//     numVertexNormals = faces.size();

//     for (int i = 0; i < faces.size(); i += 3)
//     {
//         vertices[i] = (GLfloat)vertex.at(faces.at(i)).x;
//         vertices[i + 1] = (GLfloat)vertex.at(faces.at(i)).y;
//         vertices[i + 2] = (GLfloat)vertex.at(faces.at(i)).z;

//         // textureCoordinates[i] = (GLfloat)textureCoordinate.at(faces.at(i + 1)).x;
//         // textureCoordinates[i + 1] = (GLfloat)textureCoordinate.at(faces.at(i + 1)).y;

//         // vertexNormals[i] = (GLfloat)vertexNormal.at(faces.at(i + 2)).x;
//         // vertexNormals[i + 1] = (GLfloat)vertexNormal.at(faces.at(i + 2)).y;
//         // vertexNormals[i + 2] = (GLfloat)vertexNormal.at(faces.at(i + 2)).z;
//     }
//     std::cout << " " << vertices[0] << " " << vertices[1] << " " << vertices[faces.size()] << std::endl;

//     // // Get Home Directory Folders
//     // std::string homeDirectiory = "";
//     // std::vector<std::string> objFilepathFolders = GetElementsOfLine(obj_model_filepath, '/');
//     // objFilepathFolders.erase(objFilepathFolders.end());
//     // for (int i = 0; i < objFilepathFolders.size(); i++)
//     // {
//     //     homeDirectiory += objFilepathFolders[i] + "/";
//     // }
//     // // Set home directory of the file
//     // std::string home = homeDirectiory;
//     // // Get objMTL file
//     // homeDirectiory += objMTL;
//     // homeDirectiory.at(homeDirectiory.length() - 1) = '\0';

//     // std::ifstream mtlFile(homeDirectiory);
//     // if (mtlFile.is_open())
//     // {
//     //     std::cout << "File Open: " << homeDirectiory << std::endl;
//     //     while (getline(mtlFile, line))
//     //     {
//     //         std::vector<std::string> words = GetElementsOfLine(line, ' ');

//     //         if (strcmp(words[0].c_str(), "Ns") == 0) // Get Specular value
//     //         {
//     //             Ns = std::stof(words[1]);
//     //         }
//     //         else if (strcmp(words[0].c_str(), "Ka") == 0) // Get Ambient Color
//     //         {
//     //             Ka = glm::vec3(stof(words[1]), stof(words[2]), stof(words[3]));
//     //         }
//     //         else if (strcmp(words[0].c_str(), "Kd") == 0) // Get Diffuse Color
//     //         {
//     //             Kd = glm::vec3(stof(words[1]), stof(words[2]), stof(words[3]));
//     //         }
//     //         else if (strcmp(words[0].c_str(), "Ks") == 0) // Get Specular Color
//     //         {
//     //             Ks = glm::vec3(stof(words[1]), stof(words[2]), stof(words[3]));
//     //         }
//     //         else if (strcmp(words[0].c_str(), "map_Kd") == 0) // Get Texture Map
//     //         {
//     //             std::string imgPath = home + words[1];
//     //             imgPath.pop_back();
//     //             std::cout << "Texture Path: " << imgPath.c_str() << std::endl;
//     //             imgData = stbi_load(imgPath.c_str(), &imgWidth, &imgHeight, &imgNumChannels, 0);
//     //         }
//     //     }
//     // }
//     // else
//     // {
//     //     throw std::runtime_error("Unable to open MTL file!");
//     // }
//     // mtlFile.close();

//     // for (int i = 0; i < numVertices; i += 3)
//     // {
//     //     std::cout << "point " << i / 3 + 1 << ": " << faces[i] << "," << faces[i + 1] << "," << faces[i + 2] << std::endl;
//     // }

//     std::cout << "Model Data " << std::endl;
//     std::cout << "\tModel MTL: " << objMTL << std::endl;
//     std::cout << "\tVertices: " << vertex.size() << std::endl;
//     std::cout << "\tTexture coordinates: " << textureCoordinate.size() << std::endl;
//     std::cout << "\tNormal Vectors: " << vertexNormal.size() << std::endl;
//     std::cout << "\tModel Faces: " << faces.size() / 9 << std::endl;
// }

// // GLuint VAO;
// // GLuint VBO[2];
// void Model::Send(void)
// {

//     // GLfloat vert[9][2] = {
//     //     {-0.5f, -0.9f}, {0.5f, -0.9f}, {-0.5f, 0.1f},
//     //     {0.5f, 0.1f}, {0.0f, 0.4f}, {-0.3f, -0.9f},
//     //     {-0.1f, -0.9f}, {-0.3f, -0.5f}, {-0.1f, -0.5f},
//     // };

//     // GLfloat vert[30] = {
//     //     -0.5f, -0.9f, 0.5f, -0.9f, -0.5f, 0.1f,
//     //     0.5f, -0.9f, 0.5f, 0.1f, -0.5f, 0.1f,
//     //     -0.5f, 0.1f, 0.5f, 0.1f, 0.0f, 0.4f,
//     //     -0.3f, -0.9f,-0.1f, -0.9f, -0.3f, -0.5f,
//     //     -0.1f, -0.9f, -0.1f, -0.5f,-0.3f, -0.5f,
//     // };

//     // GLuint indices[15] = {
//     //     0, 1, 2,
//     //     1, 3, 2, // Parede
//     // 	2, 3, 4, // Telhado
//     // 	5, 6, 7,
//     //     6, 8, 7 // Porta
//     // };

//     glm::mat4 Model = glm::mat4(1);
//     Model = glm::scale(Model, glm::vec3(5));
//     glm::mat4 View = glm::lookAt(
//         glm::vec3(0, 0, 10),
//         glm::vec3(0, 0, 0),
//         glm::vec3(0, 1, 0));
//     glm::mat4 Projection = glm::perspective(glm::radians(60.0f), 4 / 3.0f, 0.1f, 1000.0f);
//     glm::mat4 MVPMatrix = Projection * View * Model;

//     // Generate VertexArrayObject of the Model
//     glGenVertexArrays(1, &vertexArrayObject);
//     // Bind VertexArrayObject to insert Buffers
//     glBindVertexArray(vertexArrayObject);
//     // Generate Buffers
//     glGenBuffers(3, vertexBufferObjects);
//     for (int i = 0; i < 3 - 1; i++)
//     {
//         glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[i]);
//         if (i == 0)
//         {
//             glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);
//         }
//         else if (i == 1)
//         {
//             glBufferStorage(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, 0);
//         }
//         else
//         {
//             glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertexNormals), vertexNormals, 0);
//         }
//     }

//     ShaderInfo shaders[] = {
//         {GL_VERTEX_SHADER, "Shader/VertexShader.vert"},
//         {GL_FRAGMENT_SHADER, "Shader/FragmentShader.frag"},
//         {GL_NONE, NULL}};
//     GLuint shaderProgram = LoadShaders(shaders);
//     if (!shaderProgram)
//         exit(EXIT_FAILURE);
//     glUseProgram(shaderProgram);

//     GLint vertexPosition = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "vertexPosition");

//     glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
//     glVertexAttribPointer(vertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
//     glEnableVertexAttribArray(vertexPosition);

//     GLint mvp = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "MVP");
//     glProgramUniformMatrix4fv(shaderProgram, mvp, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
//     glEnable(GL_DEPTH_TEST);
//     glEnable(GL_CULL_FACE);
//     glEnable(GL_FRONT_AND_BACK);
// }

// void Model::Draw(glm::vec3 position, glm::vec3 orientation)
// {
//     glBindVertexArray(vertexArrayObject);
//     glDrawArrays(GL_TRIANGLES, 0, numVertices);
// }

// // TODO: make function return matrix of array of char
// static std::vector<std::string> GetElementsOfLine(const std::string line, const char element)
// {
//     std::vector<std::string> elements;
//     std::string word;
//     for (int i = 0; i < line.length(); i++)
//     {
//         char l = line[i];
//         if (l == element)
//         {
//             if (strcmp(word.c_str(), "") != 0)
//             {
//                 elements.push_back(word);
//                 word = "";
//             }
//         }
//         else
//         {
//             word += l;
//         }
//     }
//     if (strcmp(word.c_str(), "") != 0)
//     {
//         elements.push_back(word);
//         word = "";
//     }
//     return elements;
// }