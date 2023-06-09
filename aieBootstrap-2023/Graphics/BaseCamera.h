﻿#pragma once
#include <glm/glm.hpp>

///////////////////////////////////////////////////////////////////////////////////
///		
///		BaseCamera.h
///		Created: 2023-03-20
///		Author: Matthew Carver
///		Description: Parent Camera class implementation - BaseCamera
///		Modified: 2023-03-28
///		
///////////////////////////////////////////////////////////////////////////////////

class BaseCamera
{
public:
    glm::vec3 GetPosition() const;
    glm::mat4 GetWorldTransform(glm::vec3 _cameraPosition, glm::vec3 _eulerAngles, glm::vec3 _scale);
    glm::mat4 GetTransform(glm::vec3 _cameraPosition, glm::vec3 _eulerAngles, glm::vec3 _scale);
    glm::mat4 GetProjectionViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix();
    float GetAspectRatio();
    
protected:
    glm::mat4 m_projectionViewTransform;
    glm::mat4 m_worldTransform;
    glm::mat4 m_viewTransform;

    glm::vec3 m_position;
    glm::vec2 m_lastMouse;
    
    float m_aspectRatio;
    float m_theta;
    float m_phi;

    BaseCamera();
    ~BaseCamera() {}

    void Update(float _deltaTime);

    void SetPosition(glm::vec3 _position);
    void SetAspectRatio(float _width, float _height);
    void SetViewMatrix(glm::vec3 _from, glm::vec3 _to, glm::vec3 _up);
    void SetProjectionMatrix(float _fieldOfView, float _aspectRatio, float _near, float _far);
    void SetProjectionMatrix(float _fieldOfView, float _width, float _height, float _near, float _far);    
};
