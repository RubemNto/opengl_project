#include "../header/RenderEngine.h"

using namespace Graphics;

RenderEngine::RenderEngine()
{
}

RenderEngine::~RenderEngine() {}

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