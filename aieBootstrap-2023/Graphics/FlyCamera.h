#pragma once
#include "BaseCamera.h"

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
