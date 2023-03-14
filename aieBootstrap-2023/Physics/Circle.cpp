#include "Circle.h"

#include <iostream>

#include "Gizmos.h"

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color, bool _isKinematic,
               bool _isTrigger) :
    Rigidbody(CIRCLE, position, velocity, 0, mass, _isKinematic, _isTrigger)
{
    m_radius = radius;
    m_mass = mass;
    m_color = color;
    m_moment = 0.5f * mass * radius * radius;
    m_lastOrientation = m_orientation;
    m_angularVelocity = 0;
}

void Circle::Draw(float _alpha)
{   
    CalculateSmoothedPosition(_alpha);
    
    aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius + 0.5f , 12, glm::vec4(1.f,1.f, 1.f, .5f));
    aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius , 12, m_color);
    aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(0.1f, 0.1f, 0.1f, 1));    
}

bool Circle::IsInside(glm::vec2 _point)
{
    return glm::distance(_point, m_position) <= m_radius;
}
