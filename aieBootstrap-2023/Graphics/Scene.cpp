#include "Scene.h"

#include "Gizmos.h"
#include "Instance.h"

/**
 * \brief Construct a new Scene
 * \param _camera Set the camera to use for this scene
 * \param _windowSize Set the window size for this scene
 * \param _light Set the global light for this scene
 * \param _ambientLightColor Set the ambient light color for this scene
 */
Scene::Scene(BaseCamera* _camera, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLightColor):
    m_camera(_camera),
    m_windowSize(_windowSize),
    m_light(_light),
    m_ambientLightColor(_ambientLightColor)
{
}

/**
 * \brief Destroy the Scene's instances
 */
Scene::~Scene()
{
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it)
    {
        delete *it;
    }
}

/**
 * \brief Set all the point light positions + colors & draw all instances in the scene
 */
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

/**
 * \brief Add an instance to the scene
 * \param _instance The instance to add to the instance list
 */
void Scene::AddInstance(Instance* _instance)
{
    m_instances.push_back(_instance);
}

/**
 * \brief Remove an instance from the scene
 * \param _instance The instance to remove from the instance list
 */
void Scene::RemoveInstance(Instance* _instance)
{
    m_instances.remove(_instance);
}

/**
 * \brief Updates the global light direction to emulate a 'day/night' cycle
 * \param _time The time in seconds
 */
void Scene::Update(float _time)
{
    // Rotate the light to emulate a 'day/night' cycle
    m_pointLights[0].direction = glm::vec3(glm::cos(_time) * 10, 2,  ((glm::sin(_time) * 1.5f) * (glm::cos(_time) * 1.5f)));
}

/**
 * \brief Set the global light for this scene
 * \param _light The light to set as the global light
 */
void Scene::SetGlobalLight(Light _light)
{
    m_light = _light;
}

/**
 * \brief Clears all instances from the instance list
 */
void Scene::ClearInstances()
{
    m_instances.clear();
}