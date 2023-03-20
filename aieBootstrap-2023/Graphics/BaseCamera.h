#pragma once
#include <glm/glm.hpp>

class BaseCamera
{
public:
    glm::mat4 m_projectionViewTransform;
    glm::mat4 m_worldTransform;
    glm::mat4 m_viewTransform;

    glm::vec3 m_position;
    glm::vec2 m_lastMouse;
    
    float m_aspectRatio;
    float m_theta;
    float m_phi;

    BaseCamera();
    ~BaseCamera() {}

    void Update(float _deltaTime);
    
    glm::vec3 GetPosition() const;
    glm::mat4 GetWorldTransform(glm::vec3 _cameraPosition, glm::vec3 _eulerAngles, glm::vec3 _scale);
    glm::mat4 GetProjectionViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix();
    float GetAspectRatio();

    void SetPosition(glm::vec3 _position);
    void SetAspectRatio(float _width, float _height);
    void SetViewMatrix(glm::vec3 _from, glm::vec3 _to, glm::vec3 _up);
    void SetProjectionMatrix(float _fieldOfView, float _aspectRatio, float _near, float _far);
    void SetProjectionMatrix(float _fieldOfView, float _width, float _height, float _near, float _far);
    
};
