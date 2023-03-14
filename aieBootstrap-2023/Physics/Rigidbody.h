#pragma once
#include "PhysicsObject.h"
#include <glm/glm.hpp>

#include "PhysicsScene.h"
#include <functional>
#include <list>

#include "Demos.h"

class Rigidbody : public PhysicsObject
{
public:
    Rigidbody(ShapeType _shapeID, glm::vec2 _position,
              glm::vec2 _velocity, float _orientation, float _mass, bool _isKinematic = false, bool _isTrigger = false);
    ~Rigidbody() {}

    std::function<void(PhysicsObject*)> CollisionCallback;
    std::function<void(PhysicsObject*)> TriggerEnterCallback;
    std::function<void(PhysicsObject*)> TriggerExitCallback;

    void FixedUpdate(glm::vec2 _gravity, float _timeStep) override;

    // Applies forces to current &/or other actor
    void ApplyForce(glm::vec2 _force, glm::vec2 _pos);
    void ApplyForceToActor(Rigidbody* _actor2, glm::vec2 _force, glm::vec2 _contact);

    void ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact, glm::vec2* _collisionNormal = nullptr, float pen = 0);

    void CalculateSmoothedPosition(float _alpha);

    void CalculateAxes();

    glm::vec2 ToWorld(glm::vec2 _pos);
    glm::vec2 ToWorldSmoothed(glm::vec2 _localPos);

    void TriggerEnter(PhysicsObject* _actor2);
    void TriggerExit(PhysicsObject* _actor2);

    float DegreeToRadian(float _degree);
    
    //Getters
    virtual float GetKineticEnergy()
    {
        return 0.5f * m_mass * glm::length(m_velocity) * glm::length(m_velocity);
    }

    // Getters
    float GetPotentialEnergy()
        { return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition()); }
    virtual float GetEnergy()
        { return GetKineticEnergy() + GetPotentialEnergy(); }
    glm::vec2 GetPosition()
        { return m_position; }
    glm::vec2 GetVelocity()
        { return m_velocity; }
    float GetMass()
        { return m_isKinematic ? INT_MAX : m_mass; }
    float GetOrientation()
        { return m_orientation; }
    glm::vec4 GetColor()
        { return m_color; }
    float GetMoment()
        { return m_isKinematic ? INT_MAX : m_moment; }
    glm::vec2 GetLocalX()
        { return m_localX; }
    glm::vec2 GetLocalY()
        { return m_localY; }
    float GetAngularVelocity()
        { return m_angularVelocity; }
    float GetLinearDrag()
        { return m_linearDrag; }
    float GetAngularDrag()
        { return m_angularDrag; }
    bool GetIsKinematic()
        { return m_isKinematic; }
    bool GetIsTrigger()
        { return m_isTrigger; }

    //Setters
    void SetTrigger(bool _isTrigger)
        { m_isTrigger = _isTrigger; }
    void SetKinematic(bool _state)
        { m_isKinematic = _state; }
    void SetPosition(glm::vec2 _position)
        { m_position = _position; }
    void SetVelocity(glm::vec2 _velocity)
        { m_velocity = _velocity; }
    void SetMass(float _mass)
        { m_mass = _mass; }
    void SetOrientation(float _orientation)
        { m_orientation = _orientation; }
    void SetColor(glm::vec4 _color)
        { m_color = _color; }
    void SetMoment(float _moment)
        { m_moment = _moment; }
    void SetLocalX(glm::vec2 _localX)
        { m_localX = _localX; }
    void SetLocalY(glm::vec2 _localY)
        { m_localY = _localY; }
    void SetAngularVelocity(float _angularVelocity)
        { m_angularVelocity = _angularVelocity; }
    void SetLinearDrag(float _linearDrag)
        { m_linearDrag = _linearDrag; }
    void SetAngularDrag(float _angularDrag)
        { m_angularDrag = _angularDrag; }
    
protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    float m_orientation;

    glm::vec2 m_lastPosition;
    float m_lastOrientation;

    glm::vec4 m_color;
    
    float m_angularVelocity;
    float m_moment;

    glm::vec2 m_smoothedPosition;
    glm::vec2 m_smoothedLocalX;
    glm::vec2 m_smoothedLocalY;

    glm::vec2 m_localX;
    glm::vec2 m_localY;

    float m_linearDrag;
    float m_angularDrag;
    
    bool m_isKinematic;

    bool m_isTrigger;
    std::list<PhysicsObject*> m_objectsInside;
    std::list<PhysicsObject*> m_objectsInsideThisFrame;
};

inline float Rigidbody::DegreeToRadian(float _degree)
{
    return _degree * (PI / (double)180.0f);
}
