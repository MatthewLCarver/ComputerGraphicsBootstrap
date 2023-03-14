#pragma once
#include <vector>

#include "SceneObject.h"

class SolarSystem
{

public:
	SolarSystem();
	~SolarSystem();

	void Update(float _deltaTime);
	void Draw();


	std::vector<SceneObject*> m_objects = std::vector<SceneObject*>();
	
};

