#include "StationaryCamera.h"



StationaryCamera::StationaryCamera(glm::vec3 _position, glm::vec3 _rotation)
{
    m_position = _position;
    m_aspectRatio = 16.0f / 9.0f;

    SetRotation(_rotation);
}

void StationaryCamera::SetRotation(glm::vec3 _rotation)
{
    m_theta = _rotation.x;
    m_phi = _rotation.y;
}
