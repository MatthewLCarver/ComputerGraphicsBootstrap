#pragma once
#include <glm/glm.hpp>

class SimpleCamera
{
public:
    SimpleCamera();
    ~SimpleCamera(){}

    virtual void Update(float _deltaTime);
    
    glm::vec3 GetPosition() const
    { return m_position; }
    
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(float _width, float _height);
    glm::mat4 GetTransform(glm::vec3 _position,
                           glm::vec3 _eulerAngles,
                           glm::vec3 _scale);

private:
    float m_theta;
    float m_phi;

    float m_moveSpeed = 5.0f;
    float m_turnSpeed = glm::radians(180.0f);

    glm::vec3 m_position;
    
    // Last position of the mouse
    glm::vec2 m_lastMouse;
};
