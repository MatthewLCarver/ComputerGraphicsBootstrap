#pragma once
#include "PhysicsObject.h"
#include <glm/vec4.hpp>

#include "Rigidbody.h"

class Plane : public PhysicsObject
{
public:
    Plane(glm::vec2 _normal, float _distance);
    ~Plane() {}

    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);
    virtual void Draw(float _alpha);
    virtual void ResetPosition();
    void ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact);

    
    //Getters
    glm::vec2 GetNormal()
        { return m_normal;}
    float GetDistance()
        {return m_distanceToOrigin;}
    virtual float GetKineticEnergy()
        { return 0;}
    virtual float GetEnergy()
        { return 0;}

    
    
protected:
    glm::vec2 m_normal;
    glm::vec4 m_color;
    float m_distanceToOrigin;
};
