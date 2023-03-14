#include "Rigidbody.h"

#include <iostream>

#include "Demos.h"
#include "PhysicsApp.h"
#include "PhysicsObject.h"

Rigidbody::Rigidbody(ShapeType _shapeID, glm::vec2 _position, glm::vec2 _velocity, float _orientation, float _mass,
    bool _isKinematic, bool _isTrigger) :
PhysicsObject(_shapeID)
{
    m_position = _position;
    m_velocity = _velocity;
    m_orientation = _orientation;
    m_mass = _mass;
    m_lastOrientation = _orientation;
    m_lastPosition = _position;
    m_linearDrag = 0.3f;
    m_angularDrag = 0.3f;
    m_isKinematic = _isKinematic;
    m_isTrigger = _isTrigger;
}

void Rigidbody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
    //Store the local Aces
    CalculateAxes();
    m_lastOrientation = m_orientation;
    m_lastPosition = m_position;

    //Trigger Checks!
    if(m_isTrigger)
    {
        //check every object that is inside us and called triggerEnterCallback on
        //if they havent registed inside us on this frame, they must have exited
        //so remove them from out list and call TriggerExitCallback
        for(auto it = m_objectsInside.begin(); it != m_objectsInside.end(); it++)
        {
            if(std::find(m_objectsInsideThisFrame.begin(), m_objectsInsideThisFrame.end(), *it) == m_objectsInsideThisFrame.end())
            {
                if(TriggerExitCallback)
                {
                    TriggerEnterCallback(*it);
                }
                it = m_objectsInside.erase(it);
                if(it == m_objectsInside.end())
                {
                    break;
                }
            }
        }
    }
    // clear this list now for next frame
    m_objectsInsideThisFrame.clear();


    //if Kinematic, don't apply forces
    if(m_isKinematic)
    {
        m_velocity = glm::vec2(0);
        m_angularVelocity = 0;
        return;
    }

    //Update position and last position

    m_position += m_velocity * _timeStep;


    //Apply Forces
    ApplyForce(_gravity * m_mass * _timeStep, glm::vec2(0));
    //Apply angular velocity to orientation
    m_orientation += DegreeToRadian(m_angularVelocity * _timeStep);

    m_velocity -= m_velocity * m_linearDrag * _timeStep;
    m_angularVelocity -= m_angularVelocity * m_angularDrag * _timeStep;

    if(length(m_velocity) <= MIN_LINEAR_THRESHOLD)
    {
        m_velocity = glm::vec2(0, 0);
    }
    if(abs(m_angularVelocity) <= MIN_ANGULAR_THRESHOLD)
    {
        m_angularVelocity = 0;
    }
}

void Rigidbody::ApplyForce(glm::vec2 _force, glm::vec2 _pos)
{
    m_angularVelocity += (_force.y * _pos.x - _force.x * _pos.y) / GetMoment();
    m_velocity += _force / GetMass();
}

void Rigidbody::ApplyForceToActor(Rigidbody* _actor2, glm::vec2 _force, glm::vec2 _contact)
{
    _actor2->ApplyForce(_force, _contact);
    ApplyForce(-_force, _contact);
}

void Rigidbody::ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact, glm::vec2* _collisionNormal, float pen)
{
    //register that these two objects have overlapped this frame
    m_objectsInsideThisFrame.push_back(_actor2);
    _actor2->m_objectsInsideThisFrame.push_back(this);

    glm::vec2 normal = glm::normalize(_collisionNormal ? *_collisionNormal : _actor2->m_position - m_position);
    glm::vec2 perp(normal.y, -normal.x);
    glm::vec2 relativeVelocity = _actor2->GetVelocity() - m_velocity;

    float r1 = glm::dot(_contact - m_position, -perp);
    float r2 = glm::dot(_contact - _actor2->m_position, perp);

    float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;

    float v2 = glm::dot(_actor2->m_velocity, normal) + r2 * _actor2->m_angularVelocity;

    if(v1 > v2)
    {
        // calculate the effective mass at contact point for each object
        // ie how much the contact point will move due to the force applied.
        float mass1 = 1.0f / (1.0f / GetMass() + (r1 * r1) / GetMoment());
        float mass2 = 1.0f / (1.0f / _actor2->GetMass() + (r2 * r2) / _actor2->GetMoment());

        float elasticity = (GetElasticity() + _actor2->GetElasticity()) / 2.0f;

        glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
            (mass1 + mass2) * (v1 - v2) * normal;

        float kePre = GetKineticEnergy() + _actor2->GetKineticEnergy();


        if(!m_isTrigger && !_actor2->m_isTrigger)
        {
            //ApplyForce(-force, _contact - m_position);
            //_actor2->ApplyForce(force, _contact - _actor2->m_position);
            ApplyForceToActor(_actor2, force, -_contact);

            if(CollisionCallback != nullptr)
                CollisionCallback(_actor2);
            if(_actor2->CollisionCallback)
                _actor2->CollisionCallback(this);
        }
        else
        {
            TriggerEnter(_actor2);
            _actor2->TriggerEnter(this);
        }

        float kePost = GetKineticEnergy() + _actor2->GetKineticEnergy();

        float deltaKE = kePost - kePre;
        if(deltaKE > kePost * 0.01f)
            std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";

        if(pen > 0)
            PhysicsScene::ApplyContactForces(this, _actor2, normal, pen);
    }
    if(pen > 0)
        PhysicsScene::ApplyContactForces(this, _actor2, normal, pen);
}

void Rigidbody::CalculateSmoothedPosition(float _alpha)
{
    m_smoothedPosition = _alpha * m_position + (1 - _alpha) * m_lastPosition;

    float smoothedOrientation = _alpha * m_orientation + (1 - _alpha) * m_lastOrientation;
    float sn = sinf(smoothedOrientation);
    float cs = cosf(smoothedOrientation);
    m_smoothedLocalX = glm::vec2(cs, sn);
    m_smoothedLocalY = glm::vec2(-sn, cs);
}

void Rigidbody::CalculateAxes()
{
    float sn = sinf(m_orientation);
    float cs = cosf(m_orientation);
    m_localX = glm::vec2(cs, sn);
    m_localY = glm::vec2(-sn, cs);
}

glm::vec2 Rigidbody::ToWorld(glm::vec2 _pos)
{
    glm::vec2 worldPos = m_position + m_localX * _pos.x + m_localY * _pos.y;
    return worldPos;
}

glm::vec2 Rigidbody::ToWorldSmoothed(glm::vec2 _localPos)
{
    glm::vec2 worldPos = m_smoothedPosition + m_smoothedLocalX * _localPos.x + m_smoothedLocalY * _localPos.y;
    return worldPos;
}

void Rigidbody::TriggerEnter(PhysicsObject* _actor2)
{
    if(m_isTrigger && std::find(m_objectsInside.begin(), m_objectsInside.end(), _actor2) == m_objectsInside.end())
    {
        m_objectsInside.push_back(_actor2);
        if(TriggerEnterCallback != nullptr)
        {
            TriggerEnterCallback(_actor2);
        }
    }
}

void Rigidbody::TriggerExit(PhysicsObject* _actor2)
{
    if(m_isTrigger && std::find(m_objectsInside.begin(), m_objectsInside.end(), _actor2) != m_objectsInside.end())
    {
        m_objectsInside.erase(std::remove(m_objectsInside.begin(), m_objectsInside.end(), _actor2), m_objectsInside.end());
        if(TriggerExitCallback != nullptr)
        {
            TriggerExitCallback(_actor2);
        }
    }
}
