#pragma once
#include "BaseCamera.h"

class StationaryCamera: public BaseCamera
{
public:
    StationaryCamera(glm::vec3 _position, glm::vec3 _rotation);
    ~StationaryCamera() {}

    void SetRotation(glm::vec3 _rotation);
    
};
