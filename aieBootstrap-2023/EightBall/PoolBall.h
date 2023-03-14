#pragma once

#include "Circle.h"
#include "Renderer2D.h"
#include "Texture.h"

enum BallType
{
    WHITE,
    SOLID,
    STRIPED,
    BLACK
};

class PoolBall : public Circle
{
public:
    PoolBall(aie::Texture* _ballTexture, BallType _ballType, glm::vec2 _position, glm::vec2 _velocity, float _mass,
        float _radius, glm::vec4 _color, bool _isKinematic, bool _isTrigger);
    ~PoolBall();
    virtual void Draw(float alpha);
    virtual void Start() {};

    void SinkBall()
        {m_hasSunk = !m_hasSunk;}
    
    aie::Texture* GetBallTexture() 
        { return m_ballTexture; }
    BallType GetBallType() 
        { return m_ballType; }
    bool HasSunk() 
        { return m_hasSunk; }

protected:
    virtual bool IsInside(glm::vec2 _point);
    
private:
    aie::Texture* m_ballTexture;
    BallType m_ballType;

    bool m_hasSunk = false;
};
