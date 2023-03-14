#pragma once

#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include <glm/mat4x4.hpp>

#include "SolarSystem.h"

class GraphicsApp : public aie::Application {
public:

	GraphicsApp();
	virtual ~GraphicsApp();

	virtual bool Startup();
	virtual void Shutdown();

	virtual void Update(float deltaTime);
	virtual void draw();

protected:

	bool LaunchShaders();

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram	m_simpleShader;
	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform;


private:
	//SolarSystem* m_solarSystem = nullptr;
};
