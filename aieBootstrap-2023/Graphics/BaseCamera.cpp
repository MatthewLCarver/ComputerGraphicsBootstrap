#include "BaseCamera.h"

#include <glm/gtc/matrix_transform.hpp>

BaseCamera::BaseCamera()
{
    m_position = glm::vec3(-10, 2, 0);
    m_aspectRatio = 16.0f / 9.0f;
    m_theta = 0;
    m_phi = 0;
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
    return glm::translate(glm::mat4(1), m_position) *
            glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.z), glm::vec3(0, 0, 1)) *
                glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.y), glm::vec3(0, 1, 0)) *
                    glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.x), glm::vec3(1, 0, 0)) *
                        glm::scale(glm::mat4(1), _scale) *
                            glm::translate(glm::mat4(1), -_cameraPosition);
}

glm::mat4 BaseCamera::GetTransform(glm::vec3 _cameraPosition, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
    return glm::translate(glm::mat4(1), _cameraPosition) *
             glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.z), glm::vec3(0, 0, 1)) *
                 glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.y), glm::vec3(0, 1, 0)) *
                     glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.x), glm::vec3(1, 0, 0)) *
                         glm::scale(glm::mat4(1), _scale);
}



glm::mat4 BaseCamera::GetProjectionViewMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}

glm::mat4 BaseCamera::GetProjectionMatrix()
{
    return glm::perspective(glm::pi<float>() * 0.25f,
                            m_aspectRatio,
                            0.1f, 1000.0f);
}

glm::mat4 BaseCamera::GetViewMatrix()
{
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);
    glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR),
                      glm::sin(phiR),
                      glm::cos(phiR) * glm::sin(thetaR));

    return glm::lookAt(m_position,
                     m_position + forward,
                        glm::vec3(0, 1, 0));
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
    m_worldTransform = glm::inverse(m_viewTransform);
}

void BaseCamera::SetProjectionMatrix(float _fieldOfView, float _aspectRatio, float _near, float _far)
{
    m_projectionViewTransform = glm::perspective(_fieldOfView, _aspectRatio, _near, _far);
}

void BaseCamera::SetProjectionMatrix(float _fieldOfView, float _width, float _height, float _near, float _far)
{
    m_projectionViewTransform = glm::perspective(_fieldOfView, _width / _height, _near, _far);
}
