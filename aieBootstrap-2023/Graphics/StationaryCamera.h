#pragma once
#include "BaseCamera.h"

///////////////////////////////////////////////////////////////////////////////////
///		
///		StationaryCamera.h
///		Created: 2023-03-20
///		Author: Matthew Carver
///		Description: Static Camera class implementation
///		Modified: 2023-03-21
///		
///////////////////////////////////////////////////////////////////////////////////

class StationaryCamera: public BaseCamera
{
public:
    StationaryCamera(glm::vec3 _position, glm::vec3 _rotation);
    ~StationaryCamera() {}

    void Update(float _deltaTime);
    
    void SetRotation(glm::vec3 _rotation);
    
};
