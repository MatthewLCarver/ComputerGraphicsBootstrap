#include "SceneObject.h"

#include <Gizmos.h>
#include <glm/gtc/matrix_transform.hpp>

/**
 * \brief Constructor for SceneObject as a planet
 * \param _ID Set the ID of the planet
 * \param _name Set the name of the planet
 * \param _size Set the size of the planet
 * \param _distance Set the distance of the planet from the sun
 * \param _rotation Set the rotation of the planet
 * \param _speed Set the speed of the planet
 * \param _color Set the color of the planet
 * \param _parent Set the parent SceneObject of the planet
 */
SceneObject::SceneObject(int _ID, std::string _name, float _size, float _distance, float _rotation, float _speed, glm::vec4 _color, SceneObject* _parent)
{
    m_ID = _ID;
    m_name = _name;
    m_size = _size;
    m_distance = _distance;
    m_rotation = _rotation;
    m_speed = _speed;
    m_color = _color;
    _parent != nullptr ? SetParent(_parent) : m_parent = nullptr;
}

/**
 * \brief Deconstructor for the planets children (moons)
 */
SceneObject::~SceneObject()
{
    for (int i = 0; i < m_children.size();)
    {
        m_children.pop_back();
    }

    delete &m_children;
}

/**
 * \brief Update the planets position and rotation around the parent planet
 * \param _deltaTime The time between frames
 */
void SceneObject::Update(float _deltaTime)
{
    m_rotation += _deltaTime * m_speed;
    m_transform = glm::rotate(glm::mat4x4(1.0f), m_rotation, glm::vec3(0, 1, 0));
    m_transform = glm::translate(m_transform, glm::vec3(m_distance, 0, 0));
    
    m_globalTransform = GetGlobalTransform();    
    
    for (auto& child : m_children)
    {
        child->Update(_deltaTime);
    }
}

/**
 * \brief Draw the planet and its children
 */
void SceneObject::Draw()
{
    aie::Gizmos::addSphere(m_globalTransform[3], m_size, 8, 8, m_color, &m_globalTransform);
    
    for (auto& child : m_children)
    {
        child->Draw();
    }
}

/**
 * \brief Set the parent of the planet and this object as a child of the parent
 * \param _parent The parent SceneObject
 */
void SceneObject::SetParent(SceneObject* _parent)
{
    m_parent = _parent;
    m_parent->AddChild(this);
}

/**
 * \brief Adds a child to the planet
 * \param _childObject The child SceneObject
 */
void SceneObject::AddChild(SceneObject* _childObject)
{
    m_children.push_back(_childObject);
}
