#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Lights
{
    enum LightType
    {
        AMBIENT_LIGHT,
        DIRECTIONAL_LIGHT,
        POINT_LIGHT,
        SPOT_LIGHT
    };

    class Light
    {

    public:
        bool active;
        float power;
        glm::vec3 color;
        LightType type;

        Light(bool active, float power, glm::vec3 color);
        ~Light();
    };

    class AmbientLight : public Light
    {
    public:
        AmbientLight(bool active, float power, glm::vec3 color);
        ~AmbientLight();
    };

    class DirectionalLight : public Light
    {
    public:
        glm::vec3 orientation;
        DirectionalLight(glm::vec3 orientation, bool active, float power, glm::vec3 color);
        ~DirectionalLight();
    };

    class PointLight : public Light
    {
    public:
        glm::vec3 position;
        PointLight(glm::vec3 position, bool active, float power, glm::vec3 color);
        ~PointLight();
    };

    class SpotLight : public Light
    {
    public:
        glm::vec3 position;
        glm::vec3 orientation;
        float cutoffAngle;

        SpotLight(glm::vec3 position, glm::vec3 orientation, float cutoffAngle, bool active, float power, glm::vec3 color);
        ~SpotLight();
    };
}