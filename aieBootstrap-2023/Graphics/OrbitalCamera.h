#pragma once
#include "BaseCamera.h"

///////////////////////////////////////////////////////////////////////////////////
///		
///		OrbitalCamera.h
///		Created: 2023-03-20
///		Author: Matthew Carver
///		Description: Dynamic Orbital Camera class implementation
///		Modified: 2023-03-21
///		
///////////////////////////////////////////////////////////////////////////////////

class OrbitalCamera: public BaseCamera
{
public:
    OrbitalCamera();
    ~OrbitalCamera() {}

    void Update(float _deltaTime);

    void SetTarget(glm::mat4 _transform, float _distanceToTarget);

private:
    float m_distanceToTarget = 10.0f;
    float m_orbitalSpeed = 0.5f;
    glm::mat4 m_targetTransform;
};
