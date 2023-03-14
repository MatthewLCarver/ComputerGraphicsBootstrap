#pragma once
#include <glm/vec2.hpp>
#include <vector>

#include "PhysicsObject.h"

class PhysicsObject;
class Rigidbody;

class PhysicsScene
{
public:
    PhysicsScene();
    ~PhysicsScene();

    void AddActor(PhysicsObject* _actor);
    void RemoveActor(PhysicsObject* _actor);

    void Update(float _dt);
    void Draw();

    //Setters
    void SetGravity(glm::vec2 _gravity) { m_gravity = _gravity; }
    void SetTimeStep(const float _timeStep) { m_timeStep = _timeStep; }
    //Getters 
    static glm::vec2 GetGravity()
        { return m_gravity; }
    float GetTimeStep()
        { return m_timeStep; }
    std::vector<PhysicsObject*> GetActors()
        {return m_actors;}
    float GetTotalEnergy();
    
    void CheckForCollision();

    static void ApplyContactForces(Rigidbody* _body1, Rigidbody* _body2, glm::vec2 _norm, float _pen);    

    static bool Plane2Plane(PhysicsObject*, PhysicsObject*);
    static bool Plane2Circle(PhysicsObject*, PhysicsObject*);
    static bool Plane2Box(PhysicsObject*, PhysicsObject*);
    
    static bool Circle2Plane(PhysicsObject*, PhysicsObject*);
    static bool Circle2Circle(PhysicsObject*, PhysicsObject*);
    static bool Circle2Box(PhysicsObject*, PhysicsObject*);
    
    static bool Box2Plane(PhysicsObject*, PhysicsObject*);
    static bool Box2Circle(PhysicsObject*, PhysicsObject*);
    static bool Box2Box(PhysicsObject*, PhysicsObject*);
    
protected:
    static glm::vec2 m_gravity;
    float m_timeStep;
    std::vector<PhysicsObject*> m_actors;
};
