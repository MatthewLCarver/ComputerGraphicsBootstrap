#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <glm/mat4x4.hpp>

///////////////////////////////////////////////////////////////////////////////////
///		
///		SceneObject.h
///		Created: 2023-03-14
///		Author: Matthew Carver
///		Description: SceneObject class implementation for the solar system
///					 simulation.
///		Modified: 2023-03-14
///		
///////////////////////////////////////////////////////////////////////////////////

class SceneObject
{
public:
	SceneObject(int _ID, std::string _name, float _size, float _distance, float _rotation, float _speed, glm::vec4 _color, SceneObject* _parent);
	~SceneObject();


	int m_ID;
	std::string m_name;

	float m_size = 1.0f;
	float m_distance = 0.0f;
	float m_rotation = 0.0f;
	float m_speed = 0.0f;
	glm::vec4 m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	SceneObject* m_parent = nullptr;
	std::vector<SceneObject*> m_children = std::vector<SceneObject*>();

	glm::mat4x4 m_transform = glm::mat4x4(1.0f);
	glm::mat4x4 m_globalTransform = glm::mat4x4(1.0f);

	glm::mat4x4 GetGlobalTransform()
	{
		return m_parent != nullptr ? m_parent->GetGlobalTransform() * m_transform : m_transform;
	}

	void Update(float _deltaTime);
	void Draw();

	void SetParent(SceneObject* _parent);
	void AddChild(SceneObject* _childObject);
	
};

