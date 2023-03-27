#include "Scene.h"
#include "Instance.h"

Scene::Scene(BaseCamera* _camera, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLightColor):
    m_camera(_camera),
    m_windowSize(_windowSize),
    m_light(_light),
    m_ambientLightColor(_ambientLightColor)
{
}

Scene::~Scene()
{
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it)
    {
        delete *it;
    }
}


void Scene::Draw()
{
    for(int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
    {
        m_pointLightPositions[i] = m_pointLights[i].direction;
        m_pointLightColors[i] = m_pointLights[i].color;
    }
    
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it)
    {
        (*it)->Draw(this);
    }
}

void Scene::AddInstance(Instance* _instance)
{
    m_instances.push_back(_instance);
}

void Scene::RemoveInstance(Instance* _instance)
{
    m_instances.remove(_instance);
}

void Scene::Update(float _time)
{
    // Rotate the light to emulate a 'day/night' cycle
    m_light.direction = glm::normalize(glm::vec3(glm::sin(_time) * 1.5f, glm::sin(_time) * 1.5f, glm::cos(_time) * 1.5f));
}

void Scene::ClearInstances()
{
    m_instances.clear();
}
