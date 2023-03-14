#include "Plane.h"

#include <iostream>

#include "Gizmos.h"

Plane::Plane(glm::vec2 _normal, float _distance) : PhysicsObject(PLANE)
{
    //m_distanceToOrigin = 0;
    //m_normal = glm::vec2(0, 1);

    m_distanceToOrigin = _distance;
    m_normal = _normal;
}

void Plane::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
}

void Plane::ResetPosition()
{
}

void Plane::ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact)
{
    // the position at which we'll apply the force relative to the object's COM
    glm::vec2 localContact = _contact - _actor2->GetPosition();

    // the plane isn't moving, so the relative velocity is just actor2's velocity at the contact point
    glm::vec2 vRel = _actor2->GetVelocity() + _actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
    float velocityIntoPlane = glm::dot(vRel, m_normal);

    // average the elasticity of the plane and the rigidbody
    float e = (GetElasticity() + _actor2->GetElasticity()) / 2.0f;

    // this is the perpendicular distance we apply the force at relative to the COM, so Torque = F*r
    float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));

    // work out the "effective mass" - this is a combination of moment of
    // inertia and mass, and tells us how much the contact point velocity 
    // will change with the force we're applying
    float mass0 = 1.0f / (1.0f / _actor2->GetMass() + (r * r) / _actor2->GetMoment());

    float j = -(1 + e) * velocityIntoPlane * mass0;
    
    //float j = glm::dot(-(1 + e) * (vRel), m_normal) / (1 / _actor2->GetMass());
    
    glm::vec2 force = m_normal * j;

    float kePre = _actor2->GetKineticEnergy();

    _actor2->ApplyForce(force, _contact - _actor2->GetPosition());

    if (_actor2->CollisionCallback)
        _actor2->CollisionCallback(this);

    float pen = glm::dot(_contact, m_normal) - m_distanceToOrigin;
    PhysicsScene::ApplyContactForces(_actor2, nullptr, m_normal, pen);

    float kePost = _actor2->GetKineticEnergy();

    float deltaKE = kePost - kePre;
    if(deltaKE > kePost * 0.01f)
        std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";
}

void Plane::Draw(float _alpha)
{
    float lineSegmentLength = 300;
    m_color = glm::vec4(0, 1, 1, 1);
    glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
    //Easy to rotate normal through 90 degrees around z
    glm::vec2 parallel(m_normal.y, -m_normal.x);
    glm::vec4 colorFade = m_color;
    colorFade.a = 0;
    glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
    glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
    //aie::Gizmos::add2DLine(start, end, color)
    aie::Gizmos::add2DTri(start, end, start - m_normal * 10.f, m_color, m_color, colorFade);
    aie::Gizmos::add2DTri(end, end - m_normal * 10.f, start - m_normal * 10.f, m_color, colorFade, colorFade);
}
