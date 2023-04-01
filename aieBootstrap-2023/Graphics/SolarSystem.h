#pragma once
#include <vector>

#include "SceneObject.h"

///////////////////////////////////////////////////////////////////////////////////
///		
///		SolarSystem.h
///		Created: 2023-03-14
///		Author: Matthew Carver
///		Description: SolarSystem class implementation 
///		Modified: 2023-03-15
///		
///////////////////////////////////////////////////////////////////////////////////

class SolarSystem
{

public:
	SolarSystem();
	~SolarSystem();

	void Update(float _deltaTime);
	void Draw();


	std::vector<SceneObject*> m_objects = std::vector<SceneObject*>();
	
};

