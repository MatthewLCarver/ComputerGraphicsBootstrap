#include "SolarSystem.h"

#include <gl_core_4_4.h>

/**
 * \brief Creates SceneObjects for the Solar System
 */
SolarSystem::SolarSystem()
{
    m_objects.push_back(new SceneObject(0, "Sun", 2.0f, 0.0f, 1.0f, 0.1f, glm::vec4(1, 1, 0, 1), nullptr));
    
    m_objects.push_back(new SceneObject(1, "Mercury", 0.1f, 0.39f * 3, 1.0f, 47.87f / 10, glm::vec4(1.0, 0.7, 0.2, 1), m_objects.at(0)));
    m_objects.push_back(new SceneObject(2, "Venus", 0.2f, 0.72f * 2, 1.0f, 35.02f / 10, glm::vec4(1.0, 0.6, 0.5, 1) , m_objects.at(0)));
    m_objects.push_back(new SceneObject(3, "Earth", 0.25f, 1.0f * 2, 1.0f, 29.78f / 10, glm::vec4(0.0, 0.0, 1, 1), m_objects.at(0)));
    m_objects.push_back(new SceneObject(4, "Mars", 0.15f, 1.52f * 2, 1.0f, 24.077f / 10, glm::vec4(1.0, 0.1, 0.1, 1), m_objects.at(0)));
    m_objects.push_back(new SceneObject(5, "Jupiter", 0.5f, 4.0f, 1.0f, 13.07f / 10, glm::vec4(0.9, 0.5, 0.0, 1), m_objects.at(0)));
    m_objects.push_back(new SceneObject(6, "Saturn", 0.45f, 4.5f, 1.0f, 9.69f / 10, glm::vec4(0.9, 0.9, 0.5, 1), m_objects.at(0)));
    m_objects.push_back(new SceneObject(7, "Uranus", 0.35f, 5.0f, 1.0f, 6.81f / 10, glm::vec4(0.0, 0.6, 0.9, 1), m_objects.at(0)));
    m_objects.push_back(new SceneObject(8, "Neptune", 0.35f, 5.5f, 1.0f, 5.43f / 10, glm::vec4(0.0, 0.3, 0.9, 1), m_objects.at(0)));
    m_objects.push_back(new SceneObject(9, "Pluto", 0.01f, 6.0f, 1.0f, 4.74f / 10, glm::vec4(1.0, 1.0, 1.0, 1), m_objects.at(0)));

    // add moons to planets
    m_objects.push_back(new SceneObject(10, "Moon", 0.05f, 0.2f, 1.0f, 1.0f, glm::vec4(0.5, 0.5, 0.5, 1), m_objects.at(3)));
    m_objects.push_back(new SceneObject(11, "Phobos", 0.03f, 0.2f, 1.0f, 0.79f, glm::vec4(0.5, 0.5, 0.5, 1), m_objects.at(4)));
    m_objects.push_back(new SceneObject(12, "Deimos", 0.03f, -0.1f, 1.0f, 1.0f, glm::vec4(0.5, 0.5, 0.5, 1), m_objects.at(4)));
    m_objects.push_back(new SceneObject(13, "Io", 0.05f, 0.3f, 1.0f, 0.85f, glm::vec4(0.5, 0.5, 0.5, 1), m_objects.at(5)));
    m_objects.push_back(new SceneObject(14, "Europa", 0.05f, -0.45f, 1.0f, 1.5f, glm::vec4(0.5, 0.5, 0.5, 1), m_objects.at(5)));
    m_objects.push_back(new SceneObject(15, "Ganymede", 0.05f, 0.5f, 1.0f, 0.75f, glm::vec4(0.5, 0.5, 0.5, 1), m_objects.at(5)));
    m_objects.push_back(new SceneObject(16, "Callisto", 0.05f, -0.4f, 1.0f, 0.5f, glm::vec4(0.5, 0.5, 0.5, 1), m_objects.at(5)));
    m_objects.push_back(new SceneObject(17, "Titan", 0.05f, 0.2f, 1.0f, 1.35f, glm::vec4(0.5, 0.5, 0.5, 1), m_objects.at(6)));
    m_objects.push_back(new SceneObject(18, "Triton", 0.05f, 0.2f, 1.0f, 1.15f, glm::vec4(0.5, 0.5, 0.5, 1), m_objects.at(8)));
    m_objects.push_back(new SceneObject(19, "Charon", 0.05f, -0.2f, 1.0f, 0.8f, glm::vec4(0.5, 0.5, 0.5, 1), m_objects.at(8)));
    
}

/**
 * \brief Destructor for each object in the solar system
 */
SolarSystem::~SolarSystem()
{
    for (int i = 0; i < m_objects.size();)
    {
        m_objects.pop_back();
    }

    delete &m_objects;
}

/**
 * \brief Update the solar system
 * \param _deltaTime The time since the last update
 */
void SolarSystem::Update(float _deltaTime)
{
    m_objects[0]->Update(_deltaTime);
}

/**
 * \brief Draw the solar system
 */
void SolarSystem::Draw()
{
    m_objects[0]->Draw();
}
