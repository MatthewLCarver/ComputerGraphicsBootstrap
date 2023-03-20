#include "BaseCamera.h"

#include <glm/gtc/matrix_transform.hpp>

BaseCamera::BaseCamera()
{
}

void BaseCamera::Update(float _deltaTime)
{
}

glm::vec3 BaseCamera::GetPosition() const
{
    return m_position;
}

glm::mat4 BaseCamera::GetWorldTransform(glm::vec3 _cameraPosition, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
    return glm::mat4();
}

glm::mat4 BaseCamera::GetProjectionViewMatrix()
{
    return glm::mat4();
}

glm::mat4 BaseCamera::GetProjectionMatrix()
{
    return glm::mat4();
}

glm::mat4 BaseCamera::GetViewMatrix()
{
    return m_viewTransform;
}

float BaseCamera::GetAspectRatio()
{
    return m_aspectRatio;
}

void BaseCamera::SetPosition(glm::vec3 _position)
{
    m_position = _position;
}

void BaseCamera::SetAspectRatio(float _width, float _height)
{
    m_aspectRatio = _width / _height;
}

void BaseCamera::SetViewMatrix(glm::vec3 _from, glm::vec3 _to, glm::vec3 _up)
{
    m_viewTransform = glm::lookAt(_from, _to, _up);
}

void BaseCamera::SetProjectionMatrix(float _fieldOfView, float _aspectRatio, float _near, float _far)
{
    m_projectionViewTransform = glm::perspective(_fieldOfView, _aspectRatio, _near, _far);
}

void BaseCamera::SetProjectionMatrix(float _fieldOfView, float _width, float _height, float _near, float _far)
{
    m_projectionViewTransform = glm::perspective(_fieldOfView, _width / _height, _near, _far);
}
