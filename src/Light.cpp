#include "../header/Light.h"

using namespace Lights;

Light::Light(bool active, float power, glm::vec3 color)
{
    this->active = active;
    this->power = power;
    this->color = color;
}
Light::~Light() {}

AmbientLight::AmbientLight(bool active, float power, glm::vec3 color) : Light(active, power, color) { this->type = LightType::AMBIENT_LIGHT; }
AmbientLight::~AmbientLight() {}

DirectionalLight::DirectionalLight(glm::vec3 orientation, bool active, float power, glm::vec3 color) : Light(active, power, color)
{
    this->orientation = orientation;
    this->type = LightType::DIRECTIONAL_LIGHT;
}
DirectionalLight::~DirectionalLight() {}

PointLight::PointLight(glm::vec3 position, bool active, float power, glm::vec3 color) : Light(active, power, color)
{
    this->position = position;
    this->type = LightType::POINT_LIGHT;
}
PointLight::~PointLight() {}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 orientation, float cutoffAngle, bool active, float power, glm::vec3 color) : Light(active, power, color)
{
    this->position = position;
    this->orientation = orientation;
    this->cutoffAngle = cutoffAngle;
    this->type = LightType::SPOT_LIGHT;
}
SpotLight::~SpotLight() {}