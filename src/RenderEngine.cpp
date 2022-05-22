#include "../header/RenderEngine.h"
#include <iostream>

using namespace Graphics;

static std::vector<std::string> GetElementsOfLine(const std::string line, const char element)
{
    std::vector<std::string> elements; // = std::vector<std::string>(0);
    std::string word;
    for (int i = 0; i < line.length(); i++)
    {
        char l = line[i];
        if (l == element) // || i == line.length() - 1)
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

RenderEngine::RenderEngine()
{
}

RenderEngine::~RenderEngine() {}

void RenderEngine::Send(void)
{
}

void RenderEngine::Read(const std::string obj_model_filepath)
{
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>(0);
    std::vector<glm::vec2> texCoord = std::vector<glm::vec2>(0);
    std::vector<glm::vec3> normalVertices = std::vector<glm::vec3>(0);
    std::vector<std::vector<GLuint>> faces = std::vector<std::vector<GLuint>>(0);

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
                int v[3] = {0};
                int vt[3] = {-1};
                int vn[3] = {-1};
                std::vector<GLuint> newFaces = std::vector<GLuint>(0);
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
        throw std::runtime_error("Unable to open file!");
        // std::cout << "Unable to open file" << std::endl;
    }
    file.close();
    std::vector<std::string> objFilepathFolders = GetElementsOfLine(obj_model_filepath, '/');
    objFilepathFolders.erase(objFilepathFolders.end());
    for (int i = 0; i < objFilepathFolders.size(); i++)
    {
        homeDirectiory += objFilepathFolders[i];
    }
    homeDirectiory += objMTL;

    Engine::Model model = Engine::Model(obj_model_filepath);

    std::ifstream mtlFile(homeDirectiory);
    if (mtlFile.is_open())
    {
        std::cout << "File Open: " << homeDirectiory << std::endl;
        while (getline(file, line))
        {
            std::vector<std::string> words = GetElementsOfLine(line, ' ');
            if (strcmp(words[0].c_str(), "Ns") == 0)
            {
                model.setSpecularExponent(std::stof(words[1]));
            }
            else if (strcmp(words[0].c_str(), "Ka") == 0)
            {
                model.setAmbientLightReflectionCoefitient(glm::vec3(stof(words[1]), stof(words[2]), stof(words[3])));
            }
            else if (strcmp(words[0].c_str(), "Kd") == 0)
            {
                model.setDiffuseLightReflectionCoefitient(glm::vec3(stof(words[1]), stof(words[2]), stof(words[3])));
            }
            else if (strcmp(words[0].c_str(), "Ks") == 0)
            {
                model.setSpecularLightReflectionCoefitient(glm::vec3(stof(words[1]), stof(words[2]), stof(words[3])));
            }
            // else if (strcmp(words[0].c_str(), "map_Kd") == 0)
            // {

            // }
        }
    }
    else
    {
        throw std::runtime_error("Unable to open file!");
    }
    model.setVertices(vertices);
    model.setTextureCoordinates(texCoord);
    model.setVertexNormals(normalVertices);
    model.setFaces(faces);

    importedModels.push_back(model);

    std::cout << "Data: " << std::endl;
    std::cout << "Vertices MTL: " << objMTL << std::endl;
    std::cout << "Vertices: " << model.getVertices().size() << std::endl;
    std::cout << "Texture coordinates: " << model.getTextureCoordinates().size() << std::endl;
    std::cout << "Normal Vectors: " << model.getVertexNormals().size() << std::endl;
    std::cout << "Mesh Faces: " << model.getFaces().size() << std::endl;
}

void RenderEngine::print_gl_info()
{
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "\nOpenGL version " << major << '.' << minor << std::endl;

    const GLubyte *glversion = glGetString(GL_VERSION);
    const GLubyte *glvendor = glGetString(GL_VENDOR);
    const GLubyte *glrenderer = glGetString(GL_RENDERER);
    std::cout << "\nVersion:  " << glversion << std::endl
              << "Vendor:   " << glvendor << std::endl
              << "Renderer: " << glrenderer << std::endl;

    std::cout << "\nSupported GLSL versions:\n";
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "Higher supported version:\n"
              << glslversion << std::endl;
    GLint numglslversions;
    std::cout << "Other supported versions:\n";
    glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numglslversions);
    for (int n = 0; n < numglslversions; n++)
    {
        std::cout << glGetStringi(GL_SHADING_LANGUAGE_VERSION, n) << std::endl;
    }
}

void RenderEngine::init(void)
{
    // Fetch data from models loaded before call of function

    // set count of array objects
    numVertexArrayObjects = importedModels.size();
    vertexArrayObjects = new GLuint(numVertexArrayObjects);
    // set Vertex Array Objects in program
    glGenVertexArrays(numVertexArrayObjects, vertexArrayObjects);
    glBindVertexArray(vertexArrayObjects[0]);

    numBuffers = 3; // vertex    /   texture coordinate  /   normal
    buffers = new GLuint(numBuffers);
    glGenBuffers(numBuffers, buffers);

    for (int i = 0; i < numBuffers - 1; i++)
    {
        // Faz bind do VBO ao buffer GL_ARRAY_BUFFER.
        // GL_ARRAY_BUFFER � utilizado para dados de atributos de v�rtices.
        // Um VBO � criado no primeiro bind que lhe seja feito.
        // Este VBO passa a estar ativo at� que seja feito o bind a outro VBO ou seja feito o bind com valor 0.
        glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
        if (i == 0)
        {
            // Inicializa o VBO (que est� ativo) com mem�ria de tamanho imut�vel.
            // glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices) /*2 * 6 * sizeof(float)*/, vertices, 0);
            // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        }
        else if (i == 1)
        {
            // Inicializa o VBO (que est� ativo) com mem�ria de tamanho imut�vel.
            // glBufferStorage(GL_ARRAY_BUFFER, sizeof(cores) /*3 * 6 * sizeof(float)*/, cores, 0);
            // glBufferData(GL_ARRAY_BUFFER, sizeof(cores), vertices, GL_STATIC_DRAW);
        }
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[numBuffers - 1]);
    // glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 0);

    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert"},
        {GL_FRAGMENT_SHADER, "triangles.frag"},
        {GL_NONE, NULL}};

    GLuint programa = LoadShaders(shaders);
    if (!programa)
        exit(EXIT_FAILURE);
    glUseProgram(programa);

    GLint coordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition"); // Para vers�es >= 4.3
    GLint coresId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vColors");    // Para vers�es >= 4.3
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glVertexAttribPointer(coresId, 3 /*3 elementos por v�rtice*/, GL_FLOAT /*do tipo float*/, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(coordsId);
    glEnableVertexAttribArray(coresId);
}

void RenderEngine::print_error(int error, const char *description)
{
    std::cout << description << std::endl;
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float fieldOfView, float nearPlane, float farPlane, float aspect)
{
    this->position = position;
    this->direction = direction;
    this->up = up;
    this->fieldOfView = glm::radians(fieldOfView);
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    this->aspect = aspect;
    Camera::updateCamera();
}

Camera::Camera()
{
    this->position = glm::vec3(0, 0, 0);
    this->direction = glm::vec3(0, 0, 1);
    this->up = glm::vec3(0, 1, 0);
    this->nearPlane = 0.1f;
    this->farPlane = 1000.0f;
    this->fieldOfView = glm::radians(60.0f);
    this->aspect = 16.0f / 9.0f;
    Camera::updateCamera();
}

Camera::~Camera() {}

glm::vec3 Camera::getCameraDirection()
{
    return this->direction;
}

glm::vec3 Camera::getCameraPosition()
{
    return this->position;
}

glm::mat4 Camera::getCameraMatrix()
{
    return this->view_projection;
}

void Camera::setCameraPosition(glm::vec3 position)
{
    this->position = position;
}

void Camera::setCameraDirection(glm::vec3 direction)
{
    this->direction = direction;
}

void Camera::updateCamera(void)
{
    this->view = glm::lookAt(
        this->position,
        position + glm::vec3(0, 0, 1),
        this->up);
    this->projection = glm::perspective(this->fieldOfView, this->aspect, this->nearPlane, this->farPlane);
    this->view_projection = this->projection * this->view;
}