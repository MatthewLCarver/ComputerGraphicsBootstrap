#include "GraphicsApp.h"
#include <imgui.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include "Gizmos.h"
#include "Input.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsApp::GraphicsApp() {

}

GraphicsApp::~GraphicsApp() {

}

bool GraphicsApp::Startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	m_cameraPosition = glm::vec3(glm::inverse(m_viewMatrix)[3]);
	m_light.color = glm::vec3(1, 1, 0);
	m_ambientLight = glm::vec3(0.25f, 0.25f, 0.25f);
	
	//m_solarSystem = new SolarSystem();
	
	return LaunchShaders();
}

void GraphicsApp::Shutdown()
{
	Gizmos::destroy();
}

void GraphicsApp::Update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	//m_solarSystem->Update(deltaTime);
	
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	// Grab the time since the application has started
	float time = getTime();

	// use the time to add to the rotation rate variable (in radians)
	m_rotationRate += time * 0.00001f;
	if (m_rotationRate > glm::pi<float>() / 2000.0f)
		m_rotationRate -= glm::pi<float>() / 2000.0f;
	
	
	// Rotate the light to emulate a 'day/night' cycle
	m_light.direction = glm::normalize(glm::vec3(1, glm::cos(time) * 2, glm::sin(time) * 2));

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	ImGUIRefresher();
}

void GraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	
	auto pv = m_projectionMatrix * m_viewMatrix;
	
	//QuadDraw(pv * m_quadTransform);
	BoxDraw(pv * m_boxTransform, m_boxTransform);
	//BunnyDraw(pv  * m_bunnyTransform);
	//PhongDraw(pv * m_bunnyTransform, m_bunnyTransform);

	//m_solarSystem->Draw();
	
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

bool GraphicsApp::LaunchShaders()
{
#pragma region QuadMesh

	

#pragma endregion

	if(!BoxLoader())
		return false;
	/*if(!BunnyLoader())
		return false;*/
	
	return true;
}

void GraphicsApp::ImGUIRefresher()
{
	ImGui::Begin("Light Settings");
	ImGui::DragFloat3("Global Light Color", &m_light.color[0], 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Global Light Direction", &m_light.direction[0], 0.01f, -1.0f, 1.0f);
	ImGui::End();
}

bool GraphicsApp::QuadLoader()
{
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (m_simpleShader.link() == false)
	{
		printf("Simple Shader has an Error: %s\n", m_simpleShader.getLastError());
		return false;
	}
	
	// Defines as 4 vertices for the 2 triangles
	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = {  0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0,-0.5f, 1 };
	vertices[3].position = {  0.5f, 0,-0.5f, 1 };

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	m_quadMesh.Initialise(4, vertices, 6, indices);
	
	// This is a 10 'unit' wide quad
	m_quadTransform = {
		10,  0,  0, 0,
		 0, 10,  0, 0,
		 0,  0, 10, 0,
		 0,  0,  0, 1
	};

	return true;
}

void GraphicsApp::QuadDraw(glm::mat4 _pvm)
{
	// Bind the shader
	m_simpleShader.bind();
	
	//Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", _pvm);

	// Draw the quad using the Mesh's draw
	m_quadMesh.Draw();
}

bool GraphicsApp::BoxLoader()
{
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	if (m_phongShader.link() == false)
	{
		printf("Simple Shader has an Error: %s\n", m_simpleShader.getLastError());
		return false;
	}
	
	// Defines as 4 vertices for the 2 triangles
	Mesh::Vertex vertices[8];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = {  0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0,-0.5f, 1 };
	vertices[3].position = {  0.5f, 0,-0.5f, 1 };
	
	vertices[4].position = { -0.5f, 1, 0.5f, 1 };
	vertices[5].position = {  0.5f, 1, 0.5f, 1 };
	vertices[6].position = { -0.5f, 1,-0.5f, 1 };
	vertices[7].position = {  0.5f, 1,-0.5f, 1 };

	//unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };
	unsigned int indices[36] = { 0, 2, 3, 3, 1, 0, // bottom
								 4, 5, 7, 7, 6, 4, // top
								 0, 1, 5, 5, 4, 0, // front
								 2, 6, 7, 7, 3, 2, // back
								 0, 4, 2, 2, 4, 6, // left
								 1, 3, 7, 7, 5, 1  // right
	};

	m_boxMesh.Initialise(8, vertices, 36, indices);
	
	m_boxTransform = {
		5,  0,  0, 0,
		 0, 5,  0, 0,
		 0,  0, 5, 0,
		 0,  0,  0, 1
	};
	
	return true;
}

void GraphicsApp::BoxDraw(glm::mat4 _pvm, glm::mat4 _transform)
{
	// Bind the Phong shader
	m_phongShader.bind();

	m_phongShader.bindUniform("CameraPosition", m_cameraPosition);

	// Bind the directional light
	m_phongShader.bindUniform("LightDirection", m_light.direction);

	// Bind the pvm using the projection view model
	m_phongShader.bindUniform("ProjectionViewModel", _pvm);

	// Bind the transform using the model matrix
	m_phongShader.bindUniform("ModelMatrix", _transform);

	// Bind the light color
	m_phongShader.bindUniform("LightColor", (glm::vec3)m_light.color);

	// Bind the ambient light using the ambient light
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);

	// Bind the ambient, diffuse, specular and specular power
	m_phongShader.bindUniform("Ka", glm::vec3(0.5f, 0.0f, 0.3f));
	m_phongShader.bindUniform("Kd", glm::vec3(0.4f, 0.4f, 0.4f));
	m_phongShader.bindUniform("Ks", glm::vec3(0.1f, 0.1f, 0.1f));
	m_phongShader.bindUniform("specularPower", 10);

	//m_bunnyMesh.draw();


	// constantly rotate the box transform by the rotation rate
	m_boxTransform = glm::rotate(m_boxTransform, m_rotationRate, glm::vec3(0, 1, 0));
	
	
	/*// Bind the shader
	m_simpleShader.bind();
	
	//Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", _pvm);*/

	// Draw the quad using the Mesh's draw
	m_boxMesh.Draw();
}

bool GraphicsApp::BunnyLoader()
{
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");
	
	if (m_phongShader.link() == false)
	{
		printf("Color Shader has an Error: %s\n", m_phongShader.getLastError());
		return false;
	}

	if(m_bunnyMesh.load("./stanford/Bunny.obj") == false)
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}


	m_bunnyTransform = {
		0.75f,  0,  0, 0,
		0,  0.75f,  0, 0,
		0,  0,  0.75f, 0,
		0,  0,  0, 1
	};

	return true;
}

void GraphicsApp::BunnyDraw(glm::mat4 _pvm)
{
	// Bind the shader
	m_colorShader.bind();

	//Bind the transform
	m_colorShader.bindUniform("ProjectionViewModel", _pvm);

	// Bind the color
	m_colorShader.bindUniform("BaseColor", vec4(0.4f, 0.3f, 0.2f, 1));

	// Draw the bunny
	m_bunnyMesh.draw();
}

void GraphicsApp::PhongDraw(glm::mat4 _pvm, glm::mat4 _transform)
{
	// Bind the Phong shader
	m_phongShader.bind();

	m_phongShader.bindUniform("CameraPosition", m_cameraPosition);

	// Bind the directional light
	m_phongShader.bindUniform("LightDirection", m_light.direction);

	// Bind the pvm using the projection view model
	m_phongShader.bindUniform("ProjectionViewModel", _pvm);

	// Bind the transform using the model matrix
	m_phongShader.bindUniform("ModelMatrix", _transform);

	// Bind the light color
	m_phongShader.bindUniform("LightColor", (glm::vec3)m_light.color);

	// Bind the ambient light using the ambient light
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);

	m_bunnyMesh.draw();
}
