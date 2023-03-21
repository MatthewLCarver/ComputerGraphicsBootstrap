#include "Scene.h"
#include "Instance.h"

Scene::Scene(FlyCamera* _camera, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLightColor):
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