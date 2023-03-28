#include "StationaryCamera.h"



StationaryCamera::StationaryCamera(glm::vec3 _position, glm::vec3 _rotation)
{
    m_position = _position;
    m_aspectRatio = 16.0f / 9.0f;

    SetRotation(_rotation);
}

void StationaryCamera::Update(float _deltaTime)
{
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);
    
    glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR),
                      glm::sin(phiR),
                      glm::cos(phiR) * glm::sin(thetaR));

    glm::vec3 up (0, 1, 0);
    
    SetViewMatrix(m_position, m_position + forward, up);
}

void StationaryCamera::SetRotation(glm::vec3 _rotation)
{
    m_theta = _rotation.x;
    m_phi = _rotation.y;
}
