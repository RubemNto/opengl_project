#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
}