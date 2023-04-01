#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <list>

#include "BaseCamera.h"
#include "Gizmos.h"

///////////////////////////////////////////////////////////////////////////////////
///		
///		Scene.h
///		Created: 2023-03-21
///		Author: Matthew Carver
///		Description: Dynamic Fly Camera class implementation
///		Modified: 2023-03-28
///		
///////////////////////////////////////////////////////////////////////////////////

class FlyCamera;
class Instance;

const int MAX_LIGHTS = 4;

enum ActiveCameraType
{
    FLY,
    STATIONARY,
    ORBITAL
};

struct Light
{
    Light()
    {
        direction = glm::vec3(0);
        color = glm::vec3(1);
    }
    
    Light(glm::vec3 _direction, glm::vec3 _color, float _intensity)
    {
        direction = _direction;
        color = _color * intensity;
    }
    
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};

class Scene
{
public:
    Scene(BaseCamera* _camera, glm::vec2 _windowSize,
          Light& _light, glm::vec3 _ambientLightColor);
    ~Scene();

    void Draw();
    
    void AddInstance(Instance* _instance);
    void RemoveInstance(Instance* _instance);

    void AddPointLights(Light _light) { m_pointLights.push_back(_light);}
    void AddPointLights(glm::vec3 _direction, glm::vec3 _color, float _intensity)
    {
        Light light(_direction, _color, _intensity);
        m_pointLights.push_back(light);
    }

    // Getters
    BaseCamera* GetCamera()
        { return m_camera; }
    glm::vec2 GetWindowSize()
        { return m_windowSize; }
    Light GetLight()
        { return m_light; }
    glm::vec3 GetAmbientLightColor()
        { return m_ambientLightColor; }
    std::list<Instance*> GetInstances()
        { return m_instances; }
    std::vector<Light>& GetPointLights()
        { return m_pointLights; }
    int GetNumberOfPointLights()
        { return m_pointLights.size(); }
    glm::vec3* GetPointLightPositions()
    { return &m_pointLightPositions[0]; }
    glm::vec3* GetPointLightColors()
    { return &m_pointLightColors[0]; }
    ActiveCameraType GetActiveCameraType()
        { return m_activeCameraType; }

    void SetCamera(ActiveCameraType _cameraType, BaseCamera* _camera)
    {
        m_activeCameraType = _cameraType;
        m_camera = _camera;
    }

    void Update(float _deltatime);
    void SetGlobalLight(Light _light);

    void ClearInstances();
    glm::vec3 GetLightPosition()
        { return m_light.direction; }


protected:
    BaseCamera* m_camera;
    glm::vec2 m_windowSize;

    Light m_sunLight;
    std::vector<Light> m_pointLights;
    Light m_light;
    glm::vec3 m_ambientLightColor;
    std::list<Instance*> m_instances;
    ActiveCameraType m_activeCameraType = FLY;

    glm::vec3 m_pointLightPositions[MAX_LIGHTS];
    glm::vec3 m_pointLightColors[MAX_LIGHTS];

    float m_rotationRate = 0.002f;
};


