#include "BaseCamera.h"

#include <glm/gtc/matrix_transform.hpp>

/**
 * \brief Constructor
 */
BaseCamera::BaseCamera()
{
    m_position = glm::vec3(-10, 2, 0);
    m_aspectRatio = 16.0f / 9.0f;
    m_theta = 0;
    m_phi = 0;
}

/**
 * \brief Update loop for the base camera
 * \param _deltaTime The time since the last frame
 */
void BaseCamera::Update(float _deltaTime)
{
}

/**
 * \brief Get the position of the camera
 * \return The position of the camera
 */
glm::vec3 BaseCamera::GetPosition() const
{
    return m_position;
}

/**
 * \brief Get the world transform of the camera
 * \param _cameraPosition The position of the camera
 * \param _eulerAngles The euler angles of the camera
 * \param _scale The scale of the camera
 * \return The world transform of the camera
 */
glm::mat4 BaseCamera::GetWorldTransform(glm::vec3 _cameraPosition, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
    return glm::translate(glm::mat4(1), m_position) *
            glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.z), glm::vec3(0, 0, 1)) *
                glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.y), glm::vec3(0, 1, 0)) *
                    glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.x), glm::vec3(1, 0, 0)) *
                        glm::scale(glm::mat4(1), _scale) *
                            glm::translate(glm::mat4(1), -_cameraPosition);
}

/**
 * \brief Get the transform of the camera
 * \param _cameraPosition The position of the camera
 * \param _eulerAngles The euler angles of the camera
 * \param _scale The scale of the camera 
 * \return The transform of the camera
 */
glm::mat4 BaseCamera::GetTransform(glm::vec3 _cameraPosition, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
    return glm::translate(glm::mat4(1), _cameraPosition) *
             glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.z), glm::vec3(0, 0, 1)) *
                 glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.y), glm::vec3(0, 1, 0)) *
                     glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.x), glm::vec3(1, 0, 0)) *
                         glm::scale(glm::mat4(1), _scale);
}

/**
 * \brief Get the projection view matrix of the camera
 * \return The projection view matrix of the camera
 */
glm::mat4 BaseCamera::GetProjectionViewMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}

/**
 * \brief Get the projection matrix of the camera
 * \return The projection matrix of the camera
 */
glm::mat4 BaseCamera::GetProjectionMatrix()
{
    return glm::perspective(glm::pi<float>() * 0.25f,
                            m_aspectRatio,
                            0.1f, 1000.0f);
}

/**
 * \brief Get the view matrix of the camera
 * \return The view matrix of the camera
 */
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

/**
 * \brief Get the aspect ratio of the camera
 * \return The aspect ratio of the camera
 */
float BaseCamera::GetAspectRatio()
{
    return m_aspectRatio;
}

/**
 * \brief Set the position of the camera
 * \param _position The position of the camera
 */
void BaseCamera::SetPosition(glm::vec3 _position)
{
    m_position = _position;
}

/**
 * \brief Set the aspect ratio of the camera
 * \param _width The width of the camera
 * \param _height The height of the camera
 */
void BaseCamera::SetAspectRatio(float _width, float _height)
{
    m_aspectRatio = _width / _height;
}

/**
 * \brief Set the view matrix of the camera
 * \param _from The position of the camera
 * \param _to The direction the camera is looking toward
 * \param _up The up vector of the camera
 */
void BaseCamera::SetViewMatrix(glm::vec3 _from, glm::vec3 _to, glm::vec3 _up)
{
    m_viewTransform = glm::lookAt(_from, _to, _up);
    m_worldTransform = glm::inverse(m_viewTransform);
}

/**
 * \brief Set the projection matrix of the camera
 * \param _fieldOfView The field of view of the camera
 * \param _aspectRatio The aspect ratio of the camera
 * \param _near The near plane of the camera
 * \param _far The far plane of the camera
 */
void BaseCamera::SetProjectionMatrix(float _fieldOfView, float _aspectRatio, float _near, float _far)
{
    m_projectionViewTransform = glm::perspective(_fieldOfView, _aspectRatio, _near, _far);
}

/**
 * \brief Set the projection matrix of the camera whist calculating the aspect ratio
 * \param _fieldOfView The field of view of the camera
 * \param _width The width of the camera
 * \param _height The height of the camera
 * \param _near The near plane of the camera
 * \param _far The far plane of the camera
 */
void BaseCamera::SetProjectionMatrix(float _fieldOfView, float _width, float _height, float _near, float _far)
{
    m_projectionViewTransform = glm::perspective(_fieldOfView, _width / _height, _near, _far);
}
