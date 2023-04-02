#include "OrbitalCamera.h"

#include <glm/gtc/matrix_transform.hpp>

/**
 * \brief Default constructor for the OrbitalCamera class
 */
OrbitalCamera::OrbitalCamera()
{
    m_position = glm::vec3(-10, 2, 0);
    m_aspectRatio = 16.0f / 9.0f;
    m_theta = 0;
    m_phi = 0;
    m_distanceToTarget = 10;
}

/**
 * \brief Update the camera in an orbital motion around a target transform
 * \param _deltaTime The time since the last frame
 */
void OrbitalCamera::Update(float _deltaTime)
{
    // if the distance to the target is less than or greater than 0.1f then move the camera m_position toward m_targetTransform to a distance of m_distanceToTarget
    if (glm::distance(m_position, glm::vec3(m_targetTransform[3])) < m_distanceToTarget - 0.1f)
    {
        m_position += glm::normalize(glm::vec3(m_targetTransform[3]) - m_position) * _deltaTime * m_orbitalSpeed * 10.f;
    }
    else if (glm::distance(m_position, glm::vec3(m_targetTransform[3])) > m_distanceToTarget + 0.1f)
    {
        m_position -= glm::normalize(m_position - glm::vec3(m_targetTransform[3])) * _deltaTime * m_orbitalSpeed * 10.f;
    }

    m_position += glm::normalize(glm::cross(glm::vec3(m_targetTransform[3]) - m_position, glm::vec3(0, 1, 0))) * _deltaTime * m_orbitalSpeed;

    // Need to rotate to face the target transform
    
    SetViewMatrix(m_position, glm::vec3(m_targetTransform[3]), glm::vec3(0, 1, 0));
}

/**
 * \brief Set the target transform and distance to the target
 * \param _transform The target transform
 * \param _distanceToTarget The distance to the target
 */
void OrbitalCamera::SetTarget(glm::mat4 _transform, float _distanceToTarget)
{
    m_targetTransform = _transform;
    m_distanceToTarget = _distanceToTarget;
}
