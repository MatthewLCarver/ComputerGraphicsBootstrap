#include "Box.h"

#include <iostream>
#include <ostream>

#include "Gizmos.h"


Box::Box(glm::vec2 _position, glm::vec2 _velocity, float _orientation, float _mass, glm::vec2 extents, glm::vec4 _color, bool _isKinematic):
    Rigidbody(BOX, _position, _velocity, 0, _mass, _isKinematic)
{
    m_position = _position;
    m_velocity = _velocity;
    m_orientation = _orientation;
    m_lastOrientation = m_orientation;
    m_mass = _mass;
    m_extents = extents;
    m_moment = 1.0f / 3.0f * m_mass * (m_extents.x * 2) * (m_extents.y * 2);
    m_color = _color;
    m_angularVelocity = 0;
    
}

void Box::Draw(float _alpha)
{
    CalculateSmoothedPosition(_alpha);
    // draw using local axes
    glm::vec2 p1 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
        - m_smoothedLocalY * m_extents.y;
    glm::vec2 p2 = m_smoothedPosition + m_smoothedLocalX * m_extents.x
        - m_smoothedLocalY * m_extents.y;
    glm::vec2 p3 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
        + m_smoothedLocalY * m_extents.y;
    glm::vec2 p4 = m_smoothedPosition + m_smoothedLocalX * m_extents.x
        + m_smoothedLocalY * m_extents.y;
    aie::Gizmos::add2DTri(p1, p2, p4, m_color);
    aie::Gizmos::add2DTri(p1, p4, p3, m_color);

}

bool Box::IsInside(glm::vec2 _point)
{
    glm::vec2 localPoint = glm::inverse(glm::mat2(m_localX, m_localY)) * (_point - m_position);
    return (abs(localPoint.x) < m_extents.x && abs(localPoint.y) < m_extents.y);
}

bool Box::CheckBoxCorners(const Box& _box, glm::vec2& _contact, int& _numContacts, float& _pen, glm::vec2& _edgeNormal)
{
    float minX, maxX, minY, maxY;
    float boxW = _box.m_extents.x * 2;
    float boxH = _box.m_extents.y * 2;
    int numLocalContacts = 0;
    glm::vec2 localContact(0, 0);
    bool first = true;

    //loop over all corners of the other box
    for(float x = -_box.m_extents.x; x < boxW; x += boxW)
    {
        for(float y = -_box.m_extents.y; y < boxH; y += boxH)
        {
            //get the position in worldSpace
            glm::vec2 p = _box.m_position + x * _box.m_localX + y * _box.m_localY;
            //Get Position in out box's local space
            glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));

            //Update the extents in each cardinal direction in our box's space
            //(IE extents along the seperating axis)
            if(first || p0.x < minX)
                minX = p0.x;
            if(first || p0.x > maxX)
                maxX = p0.x;
            if(first || p0.y < minY)
                minY = p0.y;
            if(first || p0.y > maxY)
                maxY = p0.y;

            //if this corner is inside the box, add it to the list of contact points
            if(p0.x >= -m_extents.x && p0.x <= m_extents.x && p0.y >= -m_extents.y && p0.y <= m_extents.y)
            {
                localContact += p0;
                numLocalContacts++;
            }
            first = false;
        }
    }

    //if we lie entirely to one side of the box along one axis, we've found a seperating axis and we can exit
    if( maxX <= -m_extents.x || minX >= m_extents.x ||
        maxY <= -m_extents.y || minY >= m_extents.y)
        return false;
    if(numLocalContacts == 0)
        return false;

    bool res = false;
    _contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) / (float) numLocalContacts;
    _numContacts++;

    //find the minimum penetration vector as a penetration amount and normal
    float pen0 = m_extents.x - minX;
    if(pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _pen = pen0;
        _edgeNormal = m_localX;
        res = true;
    }
    pen0 = maxX + m_extents.x;
    if(pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _pen = pen0;
        _edgeNormal = -m_localX;
        res = true;
    }
    pen0 = m_extents.y - minY;
    if(pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _pen = pen0;
        _edgeNormal = m_localY;
        res = true;
    }
    pen0 = maxY + m_extents.y;
    if(pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _pen = pen0;
        _edgeNormal = -m_localY;
        res = true;
    }
    return res;
}
