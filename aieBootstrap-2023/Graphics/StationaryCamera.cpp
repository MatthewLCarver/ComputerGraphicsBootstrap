#include "StationaryCamera.h"

StationaryCamera::StationaryCamera()
{
    m_position = glm::vec3(-10, 2, 0);
    m_aspectRatio = 16.0f / 9.0f;
    m_theta = 0;
    m_phi = 0;
}

void StationaryCamera::SetRotation(glm::vec3 _rotation)
{
    m_theta = _rotation.x;
    m_phi = _rotation.y;
}
