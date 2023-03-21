#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <list>

class FlyCamera;
class Instance;

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
    Scene(FlyCamera* _camera, glm::vec2 _windowSize,
          Light& _light, glm::vec3 _ambientLightColor);
    ~Scene();

    void Draw();
    
    void AddInstance(Instance* _instance);
    void RemoveInstance(Instance* _instance);


    // Getters
    FlyCamera* GetCamera()
        { return m_camera; }
    glm::vec2 GetWindowSize()
        { return m_windowSize; }
    Light GetLight()
        { return m_light; }
    glm::vec3 GetAmbientLightColor()
        { return m_ambientLightColor; }
    std::list<Instance*> GetInstances()
        { return m_instances; }

protected:
    FlyCamera* m_camera;
    glm::vec2 m_windowSize;
    Light m_light;
    glm::vec3 m_ambientLightColor;
    std::list<Instance*> m_instances;
};


