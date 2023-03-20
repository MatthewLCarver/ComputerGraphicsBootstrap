#include "SimpleCamera.h"
#include <glm/ext.hpp>
#include "Input.h"

SimpleCamera::SimpleCamera()
{
    m_position = glm::vec3(-10, 2, 0);
    m_phi = 0;
    m_theta = 0;
}

void SimpleCamera::Update(float _deltaTime)
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
        m_position += forward * m_moveSpeed * _deltaTime;
    if(input->isKeyDown(aie::INPUT_KEY_S))
        m_position -= forward * m_moveSpeed *  _deltaTime;
    
    if(input->isKeyDown(aie::INPUT_KEY_A))
        m_position -= right * m_moveSpeed *  _deltaTime;
    if(input->isKeyDown(aie::INPUT_KEY_D))
        m_position += right * m_moveSpeed * _deltaTime;

    if(input->isKeyDown(aie::INPUT_KEY_Q))
        m_position += up * m_moveSpeed * _deltaTime;
    if(input->isKeyDown(aie::INPUT_KEY_E))
        m_position -= up * m_moveSpeed * _deltaTime;

    // Get the mouse coordinates
    float mouseX = input->getMouseX();
    float mouseY = input->getMouseY();

    // If the right button is held down, increment the theta and phi for rotation
    if(input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
    {
        m_theta += m_turnSpeed * (mouseX - m_lastMouse.x) * _deltaTime;
        m_phi += m_turnSpeed * (mouseY - m_lastMouse.y) * _deltaTime;
    }

    m_lastMouse = glm::vec2(mouseX, mouseY);

    if(input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
        m_moveSpeed = 10.0f;
    else
        m_moveSpeed = 5.0f;
}

glm::mat4 SimpleCamera::GetViewMatrix()
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

glm::mat4 SimpleCamera::GetProjectionMatrix(float _width, float _height)
{
    return glm::perspective(glm::pi<float>() * 0.25f,
                            _width / _height,
                            0.1f, 1000.0f);
}

glm::mat4 SimpleCamera::GetTransform(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
    return glm::mat4(0);
}
