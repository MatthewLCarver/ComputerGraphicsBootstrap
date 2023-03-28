#include "FlyCamera.h"

#include <gl_core_4_4.h>
#include <Input.h>
#include <glm/gtc/quaternion.hpp>

FlyCamera::FlyCamera()
{
    m_position = glm::vec3(-10, 2, 0);
    m_aspectRatio = 16.0f / 9.0f;
    m_theta = 0;
    m_phi = 0;
    
    m_speed = 5.0f;
    m_up = glm::vec3(0, 1, 0);
}

void FlyCamera::Update(float _deltaTime)
{
    aie::Input* input = aie::Input::getInstance();
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);

    glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR),
                      glm::sin(phiR),
                      glm::cos(phiR) * glm::sin(thetaR));

    glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));

    glm::vec3 up (0, 1, 0);


    // We will ise WASD to move and the Q & E keys to move up and down
    if(input->isKeyDown(aie::INPUT_KEY_W))
        m_position += forward * m_speed * _deltaTime;
    if(input->isKeyDown(aie::INPUT_KEY_S))
        m_position -= forward * m_speed *  _deltaTime;
    
    if(input->isKeyDown(aie::INPUT_KEY_A))
        m_position -= right * m_speed *  _deltaTime;
    if(input->isKeyDown(aie::INPUT_KEY_D))
        m_position += right * m_speed * _deltaTime;

    if(input->isKeyDown(aie::INPUT_KEY_Q))
        m_position -= up * m_speed * _deltaTime;
    if(input->isKeyDown(aie::INPUT_KEY_E))
        m_position += up * m_speed * _deltaTime;

    // Get the mouse coordinates
    glm::vec2 mousePosition = glm::vec2(input->getMouseX(), input->getMouseY());

    if(input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
    {
        m_theta += m_speed * (mousePosition.x - m_lastMouse.x) * _deltaTime;
        m_phi += m_speed * (mousePosition.y - m_lastMouse.y) * _deltaTime;
    }
    
    m_lastMouse = mousePosition;

    // Hold left shift to boost speed
    SetSpeed(input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT) ? 15.0f : 5.0f);

    // Set the view matrix
    SetViewMatrix(m_position, m_position + forward, m_up);
}

void FlyCamera::SetSpeed(float _speed)
{
    m_speed = _speed;
}
