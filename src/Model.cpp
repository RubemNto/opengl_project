#include "../header/Model.h"
#include "../header/stb_image.h"
#include "../header/LoadShaders.h"

using namespace RenderEngine;

Model::Model() {}
Model::Model(const std::string obj_model_filepath)
{
    lightEnabled = false;
    ReadModel(obj_model_filepath);
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

void Model::SetLightActive(bool value)
{
    lightEnabled = value;
}

void Model::Draw(Camera camera, glm::vec3 position, glm::vec3 orientation, float scale)
{
    glm::mat4 Model = glm::mat4(1);
    Model = glm::translate(Model, position);

    Model = glm::rotate(Model, glm::radians(orientation.x), glm::vec3(1, 0, 0));
    Model = glm::rotate(Model, glm::radians(orientation.y), glm::vec3(0, 1, 0));
    Model = glm::rotate(Model, glm::radians(orientation.z), glm::vec3(0, 0, 1));

    Model = glm::scale(Model, glm::vec3(scale));

    glBindVertexArray(vertexArrayObject);

    GLint model = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "model");
    glProgramUniformMatrix4fv(shaderProgram, model, 1, GL_FALSE, glm::value_ptr(Model));
    GLint view = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "view");
    glProgramUniformMatrix4fv(shaderProgram, view, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
    GLint projection = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "projection");
    glProgramUniformMatrix4fv(shaderProgram, projection, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

    glm::mat4 inverseView = glm::inverse(camera.GetViewMatrix());
    glm::vec3 viewDirVector = glm::vec3(inverseView[2][0], inverseView[2][1], inverseView[2][2]);
    GLint viewDir = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "viewDir");
    glProgramUniform3fv(shaderProgram, viewDir, 1, glm::value_ptr(viewDirVector));

    // define values in shader
    GLuint AmbientLight_Color = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "aLight.ambient");
    GLuint AmbientLight_Power = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "aLight.power");
    GLuint AmbientLight_Active = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "aLight.state");

    // Send Ambient Light Data
    glProgramUniform3fv(shaderProgram, AmbientLight_Color, 1, glm::value_ptr(ambientLight->color));
    glProgramUniform1f(shaderProgram, AmbientLight_Power, ambientLight->power);
    glProgramUniform1i(shaderProgram, AmbientLight_Active, ambientLight->active);

    GLuint DirectionalLight_Color = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "dLight.ambient");
    GLuint DirectionalLight_Diffuse = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "dLight.diffuse");
    GLuint DirectionalLight_Specular = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "dLight.specular");
    GLuint DirectionalLight_Orientation = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "dLight.orientation");
    GLuint DirectionalLight_Power = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "dLight.power");
    GLuint DirectionalLight_Active = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "dLight.state");

    glProgramUniform3fv(shaderProgram, DirectionalLight_Color, 1, glm::value_ptr(directionalLight->color));
    glProgramUniform3fv(shaderProgram, DirectionalLight_Diffuse, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
    glProgramUniform3fv(shaderProgram, DirectionalLight_Specular, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
    glProgramUniform3fv(shaderProgram, DirectionalLight_Orientation, 1, glm::value_ptr(directionalLight->orientation));
    glProgramUniform1f(shaderProgram, DirectionalLight_Power, directionalLight->power);
    glProgramUniform1i(shaderProgram, DirectionalLight_Active, directionalLight->active);

    GLuint PointLight_Color = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pLight.ambient");
    GLuint PointLight_Diffuse = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pLight.diffuse");
    GLuint PointLight_Specular = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pLight.specular");
    GLuint PointLight_Power = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pLight.power");
    GLuint PointLight_Position = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pLight.position");
    GLuint PointLight_Active = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pLight.state");

    GLuint PointLight_Constant = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pLight.constant");
    GLuint PointLight_Linear = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pLight.linear");
    GLuint PointLight_Quadratic = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pLight.quadratic");

    glProgramUniform3fv(shaderProgram, PointLight_Color, 1, glm::value_ptr(pointLight->color));
    glProgramUniform3fv(shaderProgram, DirectionalLight_Diffuse, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
    glProgramUniform3fv(shaderProgram, DirectionalLight_Specular, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    glProgramUniform3fv(shaderProgram, PointLight_Position, 1, glm::value_ptr(pointLight->position));
    glProgramUniform1f(shaderProgram, PointLight_Power, pointLight->power);
    glProgramUniform1i(shaderProgram, PointLight_Active, pointLight->active);

    glProgramUniform1f(shaderProgram, PointLight_Constant, 1.0f);
    glProgramUniform1f(shaderProgram, PointLight_Linear, 0.7f);
    glProgramUniform1f(shaderProgram, PointLight_Quadratic, 0.4f);

    // GLuint SpotLight_Color = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "sLight.ambient");
    // GLuint SpotLight_Diffuse = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "sLight.diffuse");
    // GLuint SpotLight_Specular = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "sLight.specular");

    // GLuint SpotLight_Power = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "sLight.power");
    // GLuint SpotLight_Position = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "sLight.position");
    // GLuint SpotLight_Direction = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "sLight.orientation");
    // GLuint SpotLight_CutoffAngle = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "sLight.cutoffAngle");
    // GLuint SpotLight_Active = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "sLight.active");

    // glProgramUniform3fv(shaderProgram, SpotLight_Color, 1, glm::value_ptr(spotLight->color));
    // glProgramUniform3fv(shaderProgram, SpotLight_Diffuse, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
    // glProgramUniform3fv(shaderProgram, SpotLight_Specular, 1,glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    // glProgramUniform3fv(shaderProgram, SpotLight_Position, 1, glm::value_ptr(spotLight->position));
    // glProgramUniform3fv(shaderProgram, SpotLight_Direction, 1, glm::value_ptr(spotLight->orientation));

    // glProgramUniform3f(shaderProgram, PointLight_Power, pointLight->position.x, pointLight->position.y, pointLight->position.z);
    // glProgramUniform1f(shaderProgram, PointLight_Position, pointLight->power);
    // glProgramUniform1i(shaderProgram, PointLight_Active, pointLight->active);

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
    for (int i = 0; i < 3; i++)
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

    GLint vertexTexturePosition = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "vertexTexturePosition");

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
    glVertexAttribPointer(vertexTexturePosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexTexturePosition);

    GLint vertexNormal = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "vertexNormal");
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[2]);
    glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexNormal);

    GLint locationTexSampler = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "texSampler");
    glProgramUniform1i(shaderProgram, locationTexSampler, 0);

    GLint Ns = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "material.Ns");
    glProgramUniform1f(shaderProgram, Ns, materials.at("Iron_Man").Ns);
    GLint Ka = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "material.Ka");
    glProgramUniform3f(shaderProgram, Ka, materials.at("Iron_Man").Ka.x, materials.at("Iron_Man").Ka.y, materials.at("Iron_Man").Ka.z);
    GLint Kd = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "material.Kd");
    glProgramUniform3f(shaderProgram, Kd, materials.at("Iron_Man").Kd.x, materials.at("Iron_Man").Kd.y, materials.at("Iron_Man").Kd.z);
    GLint Ks = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "material.Ks");
    glProgramUniform3f(shaderProgram, Ks, materials.at("Iron_Man").Ks.x, materials.at("Iron_Man").Ks.y, materials.at("Iron_Man").Ks.z);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FRONT);
}

void Model::LoadTexture(const std::string file_name)
{
    GLuint textureName = 0;

    glGenTextures(1, &textureName);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, textureName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *imageData = stbi_load(file_name.c_str(), &width, &height, &nChannels, 0);

    if (imageData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(imageData);
    }
    else
    {
        cout << "Unable to load Texture!" << endl;
    }
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
                elements.at(1).pop_back();
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
                elements.at(1).pop_back();
                materials.at(MaterialName).imgName = elements.at(1);
                materials.at(MaterialName).imagePath = HomeFolder + elements.at(1);
                LoadTexture(materials.at(MaterialName).imagePath);
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