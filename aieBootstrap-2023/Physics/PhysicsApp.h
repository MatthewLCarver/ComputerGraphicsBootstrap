#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <vector>
#include <glm/detail/type_vec.hpp>

#include "Circle.h"
#include "Input.h"

class Box;
class PhysicsScene;
class PhysicsObject;
class Circle;

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool Startup();
	virtual void Shutdown();

	virtual void Update(float _deltaTime);
	virtual void draw();
	
	glm::vec2 ScreenToWorld(glm::vec2 _screenPos);

protected:
	//aie
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	//physics
	PhysicsScene* m_physicsScene;
	std::vector<PhysicsObject*> m_objects;

private:
	const float m_extents = 250;
	const float m_aspectRatio = 16.0f / 9.0f;


public:
	void DemoStartUp(int _num);
	void DemoUpdates(aie::Input* _input, float _dt);
	float GetAngleBetweenPoints(glm::vec2 _point1, glm::vec2 _point2);
	float RadianToDegree(float _radian);
	float DegreeToRadian(float _degree);
};



