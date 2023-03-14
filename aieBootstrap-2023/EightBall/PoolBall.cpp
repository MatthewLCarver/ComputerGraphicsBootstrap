#include "PoolBall.h"

#include <Gizmos.h>


PoolBall::PoolBall(aie::Texture* _ballTexture, BallType _ballType, glm::vec2 _position, glm::vec2 _velocity, float _mass, float _radius,
                   glm::vec4 _color, bool _isKinematic, bool _isTrigger): 
    Circle(_position, _velocity, _mass, _radius, _color, _isKinematic, _isTrigger)
{
    m_ballTexture = _ballTexture;
    m_ballType = _ballType;

    m_angularDrag = 1;
    m_linearDrag = 0.35f;
}

PoolBall::~PoolBall()
{
    delete m_ballTexture;
}

void PoolBall::Draw(float alpha)
{
    /*CalculateSmoothedPosition(alpha);
    
    aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius + 0.5f , 12, glm::vec4(1.f,1.f, 1.f, .5f));
    aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius , 12, m_color);
    aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(0.1f, 0.1f, 0.1f, 1));    */
}

bool PoolBall::IsInside(glm::vec2 _point)
{
    return glm::distance(_point, m_position) <= m_radius;
}
