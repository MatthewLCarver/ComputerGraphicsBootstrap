#pragma once
#include "Rigidbody.h"

class Box : public Rigidbody
{
public:
    Box(glm::vec2 _position, glm::vec2 _velocity, float _orientation,float _mass, glm::vec2 extents, glm::vec4 _color, bool _isKinematic);
    ~Box() {}

    virtual void Draw(float _alpha);
    virtual bool IsInside(glm::vec2 _point);
    bool CheckBoxCorners(const Box& _box, glm::vec2& _contact, int& _numContacts, float& _pen, glm::vec2& _edgeNormal);

    //Getters
    glm::vec2 GetExtents() { return m_extents; }
    float GetWidth() { return m_extents.x * 2; }
    float GetHeight() { return m_extents.y * 2; }

    //Setters
    void SetExtents(glm::vec2 _extents) { m_extents = _extents; }

protected:
    glm::vec2 m_extents;

};
