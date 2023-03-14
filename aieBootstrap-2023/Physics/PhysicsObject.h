#pragma once
#include <glm/vec2.hpp>

enum ShapeType
{
    JOINT = -1,
    PLANE = 0,
    CIRCLE,
    BOX,
};

const int SHAPE_COUNT = 3;

class PhysicsObject
{
protected:
    PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID)
    {
        m_elasticity = .8f;
    }

public:
    
    virtual ~PhysicsObject() = default;
    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep) = 0;
    virtual void Draw(float _alpha) = 0;

    virtual void ResetPosition(){}
    virtual bool IsInside(glm::vec2 _point)
        { return false; }
    
    void SetElasticity(float _elasticity)
        { m_elasticity = _elasticity; }

    //Getter
    ShapeType GetShapeID()
        { return m_shapeID; }
    virtual float GetKineticEnergy() =0;
    virtual float GetEnergy() =0;
    float GetElasticity()
        { return m_elasticity; }

protected:
    ShapeType m_shapeID;
    float m_elasticity;
    
};
