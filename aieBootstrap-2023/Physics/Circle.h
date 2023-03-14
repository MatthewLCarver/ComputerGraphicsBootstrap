#pragma once
#include "Rigidbody.h"

class Circle : public Rigidbody
{
public:
    Circle(glm::vec2 _position, glm::vec2 _velocity, float _mass,
        float _radius, glm::vec4 _color, bool _isKinematic, bool _isTrigger);
    ~Circle() {}

    virtual void Draw(float _alpha);
    virtual void Start() {}
    virtual bool IsInside(glm::vec2 _point);

    //Getters
    float GetRadius() { return m_radius; }
    glm::vec4 GetColor() { return m_color; }
    bool GetCollisionState() { return hasCollidedWithCircle;}
    //Setters
    void SetRadius(float _radius) { m_radius = _radius; }
    void SetCollisionState(bool _state) {hasCollidedWithCircle = _state;}

protected:
    float m_radius;
    glm::vec4 m_color;
    bool hasCollidedWithCircle = false;
};
