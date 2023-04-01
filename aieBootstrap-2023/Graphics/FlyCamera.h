#pragma once
#include "BaseCamera.h"

///////////////////////////////////////////////////////////////////////////////////
///		
///		FlyCamera.h
///		Created: 2023-03-20
///		Author: Matthew Carver
///		Description: Dynamic Fly Camera class implementation
///		Modified: 2023-03-21
///		
///////////////////////////////////////////////////////////////////////////////////

class FlyCamera : public BaseCamera 
{
public:
    FlyCamera();
    ~FlyCamera() {}

    // override the update function
    void Update(float _deltaTime);
    
    void SetSpeed(float _speed);
    
protected:
    float m_speed;
    glm::vec3 m_up;
};
