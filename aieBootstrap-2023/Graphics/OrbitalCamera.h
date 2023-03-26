#pragma once
#include "BaseCamera.h"

class OrbitalCamera: public BaseCamera
{
public:
    OrbitalCamera();
    ~OrbitalCamera() {}

    void Update(float _deltaTime);

    void SetTarget(glm::mat4 _transform, float _distanceToTarget);

private:
    float m_distanceToTarget = 10.0f;
    float m_orbitalSpeed = 5.0f;
    glm::mat4 m_targetTransform;
};
