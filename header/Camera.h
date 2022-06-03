#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
    glm::mat4 ViewProjection;
    glm::mat4 View;
    glm::mat4 Projection;

public:
    glm::vec3 position;
    glm::vec3 orientation;
    glm::vec3 target;
    float FOV;
    float aspectRatio;
    float nearPlane, farPlane;

    Camera(
        glm::vec3 position,
        glm::vec3 orientation,
        glm::vec3 target,
        float FOV,
        float aspectRatio,
        float nearPlane,
        float farPlane);
    ~Camera();
    glm::mat4 GetViewProjectionMatrix();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    void UpdateCamera();
};
