#include "OrbitalCamera.h"

OrbitalCamera::OrbitalCamera()
{
    m_position = glm::vec3(-10, 2, 0);
    m_aspectRatio = 16.0f / 9.0f;
    m_theta = 0;
    m_phi = 0;
}

void OrbitalCamera::Update(float _deltaTime)
{
    m_theta += _deltaTime * m_orbitalSpeed;
    m_position.x = m_distanceToTarget * cos(m_theta);
    m_position.z = m_distanceToTarget * sin(m_theta);

    // rotate to look at the target transform
    glm::vec3 targetPosition = glm::vec3(m_targetTransform[3]);
    glm::vec3 direction = glm::normalize(targetPosition - m_position);
    m_phi = asin(direction.y);
    m_theta = atan2(direction.z, direction.x);
    
    glm::vec3 up = glm::vec3(m_targetTransform[1]);
    SetViewMatrix(m_position, m_position + direction, up);
}

void OrbitalCamera::SetTarget(glm::mat4 _transform, float _distanceToTarget)
{
    m_targetTransform = _transform;
    m_distanceToTarget = _distanceToTarget;
}
