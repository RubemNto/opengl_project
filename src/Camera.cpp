#include "../header/Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 orientation, glm::vec3 target, float FOV, float aspectRatio, float nearPlane, float farPlane)
{
    this->position = position;
    this->orientation = orientation;
    this->FOV = FOV;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    this->target = target;
    UpdateCamera();
}

Camera::~Camera() {}

glm::mat4 Camera::GetViewProjectionMatrix()
{
    return ViewProjection;
}

glm::mat4 Camera::GetViewMatrix()
{
    return View;
}
glm::mat4 Camera::GetProjectionMatrix()
{
    return Projection;
}

void Camera::UpdateCamera()
{
    glm::mat4 View = glm::lookAt(
        position,
        target,
        orientation);
    glm::mat4 Projection = glm::perspective(FOV, aspectRatio, nearPlane, farPlane);
    ViewProjection = Projection * View;
}
