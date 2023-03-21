#include "GraphicsApp.h"

#include <gl_core_4_4.h>
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
	m_light.color = glm::vec3(1, 1, 1);
	m_ambientLight = glm::vec3(0.25f, 0.25f, 0.25f);
	
	//m_solarSystem = new SolarSystem();
	
	return LaunchShaders();
}

void GraphicsApp::Shutdown()
{
	Gizmos::destroy();
	delete m_scene;
}

void GraphicsApp::Update(float _deltaTime) {

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

	//m_solarSystem->Update(_deltaTime);
	
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	// Grab the time since the application has started
	float time = getTime();

	// use the time to add to the rotation rate variable (in radians)
	m_rotationRate = .002f;
	
	// Rotate the light to emulate a 'day/night' cycle
	m_light.direction = glm::normalize(glm::vec3(1, glm::sin(time) * 1.5f, glm::cos(time) * 1.5f));

	m_flyCamera.Update(_deltaTime);

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	ImGUIRefresher();
}

void GraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	/*// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	*/

	// create simple camera transforms
	m_viewMatrix = m_flyCamera.GetViewMatrix();
	//glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	//m_projectionMatrix = m_flyCamera.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_projectionMatrix = m_flyCamera.GetProjectionMatrix();
	
	auto pv = m_projectionMatrix * m_viewMatrix;

	m_scene->Draw();
	
	//QuadDraw(pv * m_quadTransform);
	QuadTextureDraw(pv * m_quadTransform);
	//IcosahedronDraw(pv * m_icosahedronTransform, m_icosahedronTransform);
	//SpearDraw(pv * m_spearTransform, m_spearTransform);
	//SwordDraw(pv * m_swordTransform, m_swordTransform);

	//ObjDraw(pv, m_spearTransform, &m_spearMesh);
	//ObjDraw(pv, m_swordTransform, &m_swordMesh);
	//BunnyDraw(pv  * m_bunnyTransform);
	//PhongDraw(pv * m_bunnyTransform, m_bunnyTransform);

	//BoxDraw(pv * m_boxTransform, m_boxTransform);
	//CylinderDraw(pv * m_cylinderTransform, m_cylinderTransform);

	//m_solarSystem->Draw();

	
	
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

}

bool GraphicsApp::LaunchShaders()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	m_normalLitShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalLit.vert");
	m_normalLitShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalLit.frag");
	
	if (m_normalLitShader.link() == false)
	{
		printf("Normal Lit Phong has an Error: %s\n", m_texturedShader.getLastError());
		return false;
	}
	if(!QuadTextureLoader())
		return false;
	/*if(!IcosahedronLoader())
		return false;
	*/
	/*if(!CylinderLoader(10, 1.0f, 0.50f))
		return false;*/
	//if(!BoxLoader())
	//	return false;
	/*if(!BunnyLoader())
		return false;*/
	
	if(!SpearLoader())
		return false;
	if(!SwordLoader())
		return false;
	
	Light light;
	light.color = glm::vec3(1, 1, 1);
	
	m_scene = new Scene(&m_flyCamera, glm::vec2(getWindowWidth(), getWindowHeight()),
	light, m_ambientLight);

	m_scene->AddInstance(new Instance(m_spearTransform, &m_spearMesh, &m_normalLitShader));
	m_scene->AddInstance(new Instance(m_swordTransform, &m_swordMesh, &m_normalLitShader));
	
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
		printf("Box Phong Shader has an Error: %s\n", m_simpleShader.getLastError());
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
	m_phongShader.bindUniform("Ka", glm::vec3(1.0f, 0.0f, 0.0f));
	m_phongShader.bindUniform("Kd", glm::vec3(1.0f, 0.4f, 0.4f));
	m_phongShader.bindUniform("Ks", glm::vec3(0.1f, 0.1f, 0.1f));
	m_phongShader.bindUniform("specularPower", 10);

	//m_bunnyMesh.draw();

	// Rotates the transform
	m_boxTransform = glm::rotate(m_boxTransform, m_rotationRate, glm::vec3(0, 1, 0));
	
	
	/*// Bind the shader
	m_simpleShader.bind();
	
	//Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", _pvm);*/

	// Draw the quad using the Mesh's draw
	m_boxMesh.Draw();
}

bool GraphicsApp::CylinderLoader(int _segments, float _height, float _radius)
{
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	if(m_phongShader.link() == false)
	{
		printf("Cylinder Phong Shader has an Error: %s\n", m_simpleShader.getLastError());
		return false;
	}
	
	Mesh::Vertex* vertices = new Mesh::Vertex[_segments * 2 + 2];
	for (int i = 0; i < _segments; ++i)
	{
		float theta = (i / (float)_segments) * glm::pi<float>() * 2.0f;
		vertices[i].position = { cos(theta) * _radius, 0, sin(theta) * _radius, 1 };
		vertices[i + _segments].position = { cos(theta) * _radius, _height, sin(theta) * _radius, 1 };
	}
	
	unsigned int* indices = new unsigned int[_segments * 6 + _segments * 6];
	for (int i = 0; i < _segments; ++i)
	{
		indices[i * 6 + 0] = i;
		indices[i * 6 + 1] = i + _segments;
		indices[i * 6 + 2] = (i + 1) % _segments;
		indices[i * 6 + 3] = (i + 1) % _segments;
		indices[i * 6 + 4] = i + _segments;
		indices[i * 6 + 5] = (i + 1) % _segments + _segments;
	}

	// cap the top and bottom of the cylinder	

	m_cylinderMesh.Initialise(_segments * 2 + 2, vertices, _segments * 6 + _segments * 6, indices);
	m_cylinderTransform = {
		5,  0,  0, 0,
		0, 5,  0, 0,
		0,  0, 5, 0,
		0,  0,  0, 1
	};

	return true;
}

void GraphicsApp::CylinderDraw(glm::mat4 _pvm, glm::mat4 _transform)
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
	m_phongShader.bindUniform("Ka", glm::vec3(1.0f, 0.0f, 0.0f));
	m_phongShader.bindUniform("Kd", glm::vec3(1.0f, 0.4f, 0.4f));
	m_phongShader.bindUniform("Ks", glm::vec3(0.1f, 0.1f, 0.1f));
	m_phongShader.bindUniform("specularPower", 10);

	// Rotates the transform
	m_cylinderTransform = glm::rotate(m_cylinderTransform, m_rotationRate, glm::vec3(0, 1, 0));

	// Draw the quad using the Mesh's draw
	m_cylinderMesh.Draw();
}

bool GraphicsApp::IcosahedronLoader()
{
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	if(m_phongShader.link() == false)
	{
		printf("Cylinder Phong Shader has an Error: %s\n", m_simpleShader.getLastError());
		return false;
	}
	
	Mesh::Vertex* vertices = new Mesh::Vertex[12];
	// create a vertex a the centre top, then create 5 vertices around the top, then 5 vertices around the bottom rotating them by 36 degrees, then a vertex at the centre bottom
	// maintain equilateral triangles
	vertices[0].position = { 0, 1, 0, 1 };

	// top 5 vertices
	for (int i = 0; i < 5; ++i)
	{
		float theta = (i / (float)5) * glm::pi<float>() * 2.0f;
		vertices[i + 1].position = { cos(theta) * 0.5f, 0.5f, sin(theta) * 0.5f, 1 };
	}

	// create the bottom 5 vertices rotated by 36 degrees from the upper 5
	for (int i = 0; i < 5; ++i)
	{
		float theta = (i / (float)5) * glm::pi<float>() * 2.0f;
		vertices[i + 6].position = { cos(theta + glm::pi<float>() / 5) * 0.5f, -0.5f, sin(theta + glm::pi<float>() / 5) * 0.5f, 1 };
	}
	
	vertices[11].position = { 0, -1, 0, 1 };
	
	
	// create and assign the indices of the d20 in a loop
	unsigned int* indices = new unsigned int[60];
	// arrange all the vertices into a sphere shape with the given vertices
	// top 5 vertices
	for (int i = 0; i < 5; ++i)
	{
		indices[i * 6 + 0] = 0;
		indices[i * 6 + 1] = i + 1;
		indices[i * 6 + 2] = (i + 1) % 5 + 1;
	}
	// bottom 5 vertices
	for (int i = 0; i < 5; ++i)
	{
		indices[i * 6 + 15] = 11;
		indices[i * 6 + 16] = i + 6;
		indices[i * 6 + 17] = (i + 1) % 5 + 6;
	}
	// middle 10 vertices
	for (int i = 0; i < 5; ++i)
	{
		indices[i * 6 + 30] = i + 1;
		indices[i * 6 + 31] = i + 6;
		indices[i * 6 + 32] = (i + 1) % 5 + 1;

		indices[i * 6 + 33] = (i + 1) % 5 + 1;
		indices[i * 6 + 34] = i + 6;
		indices[i * 6 + 35] = (i + 1) % 5 + 6;
	}
	


	

	// initialise the icosahedron
	m_icosahedronMesh.Initialise(12, vertices, 60, indices);
	
	m_icosahedronTransform = {
		2,  0,  0, 0,
		0, 2,  0, 0,
		0,  0, 2, 0,
		0,  0,  0, 1
	};

	m_icosahedronTransform = glm::translate(m_icosahedronTransform, glm::vec3(0, 2, 5));

	return true;
}

void GraphicsApp::IcosahedronDraw(glm::mat4 _pvm, glm::mat4 _transform)
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
	m_phongShader.bindUniform("Ka", glm::vec3(1.0f, 0.0f, 0.0f));
	m_phongShader.bindUniform("Kd", glm::vec3(1.0f, 0.4f, 0.4f));
	m_phongShader.bindUniform("Ks", glm::vec3(0.1f, 0.1f, 0.1f));
	m_phongShader.bindUniform("specularPower", 10);

	// Rotates the transform
	m_icosahedronTransform = glm::rotate(m_icosahedronTransform, m_rotationRate, glm::vec3(0, 1, 0));

	// Draw the quad using the Mesh's draw
	m_icosahedronMesh.Draw();
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

bool GraphicsApp::SpearLoader()
{
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");
	
	if (m_texturedShader.link() == false)
	{
		printf("Color Shader has an Error: %s\n", m_texturedShader.getLastError());
		return false;
	}

	if(m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soul Spear Mesh Error!\n");
		return false;
	}

	m_spearTransform = {
		0.75f,  0,  0, 0,
		0,  0.75f,  0, 0,
		0,  0,  0.75f, 0,
		0,  0,  0, 1
	};

	return true;
}

void GraphicsApp::SpearDraw(glm::mat4 _pvm, glm::mat4 _transform)
{
	// Bind the shader
	m_texturedShader.bind();
	
	//Bind the transform
	m_texturedShader.bindUniform("ProjectionViewModel", _pvm);

	// Bind the diffuse texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);

	// Bind the texture to a specific location
	m_spearTexture.bind(0);

	// Rotates the transform
	m_spearTransform = glm::rotate(m_spearTransform, m_rotationRate, glm::vec3(0, 1, 0));

	// Draw the bunny
	m_spearMesh.draw();
}

bool GraphicsApp::SwordLoader()
{
	if(m_swordMesh.load("./artorias/sword.obj", true, true) == false)
	{
		printf("Sword Mesh Error!\n");
		return false;
	}

	m_swordTransform = {
		0.25f,  0,  0, 0,
		0,  0.25f,  0, 0,
		0,  0,  0.25f, 0,
		0,  0,  0, 1
	};

	m_swordTransform = glm::translate(m_swordTransform, glm::vec3(15.0f, 13.0f, 0.0f));

	return true;
}

void GraphicsApp::SwordDraw(glm::mat4 _pvm, glm::mat4 _transform)
{
	// Bind the shader
	m_normalLitShader.bind();	

	m_phongShader.bindUniform("CameraPosition",
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	// Bind the directional light
	m_phongShader.bindUniform("LightDirection", m_light.direction);
	// Bind the light color
	m_phongShader.bindUniform("LightColor", (glm::vec3)m_light.color);
	// Bind the ambient light using the ambient light
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
	
	// Bind the pvm using the projection view model
	m_phongShader.bindUniform("ProjectionViewModel", _pvm);

	// Bind the diffuse texture location
	m_normalLitShader.bindUniform("diffuseTexture", 0);
	
	// Bind the transform using the model matrix
	m_phongShader.bindUniform("ModelMatrix", _transform);

	

	
	m_normalLitShader.bindUniform("normalTexture", 0);
	
	// Bind the texture to a specific location
	m_swordTexture.bind(0);
	m_swordNormalTexture.bind(1);

	// Rotates the transform
	m_swordTransform = glm::rotate(m_swordTransform, m_rotationRate, glm::vec3(0, 1, 0));

	// Draw the bunny
	m_swordMesh.draw();
}

bool GraphicsApp::QuadTextureLoader()
{
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");

	if (m_texturedShader.link() == false)
	{
		printf("Textured Shader has an Error: %s\n", m_texturedShader.getLastError());
		return false;
	}
	
	if(m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load the grid texture correctly\n");
		return false;
	}

	
	m_quadMesh.InitialiseQuad();
	
	
	// This is a 10 'unit' wide quad
	m_quadTransform = {
		10,  0,  0, 0,
		 0, 10,  0, 0,
		 0,  0, 10, 0,
		 0,  0,  0, 1
	};

	return true;
}

void GraphicsApp::QuadTextureDraw(glm::mat4 _pvm)
{
	// Bind the shader
	m_texturedShader.bind();
	
	//Bind the transform
	m_texturedShader.bindUniform("ProjectionViewModel", _pvm);

	// Bind the diffuse texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);

	// Bind the texture to a specific location
	m_gridTexture.bind(0);

	// Draw the quad using the Mesh's draw
	m_quadMesh.Draw();
}

void GraphicsApp::ObjDraw(glm::mat4 _pv, glm::mat4 _transform, aie::OBJMesh* _mesh)
{
	m_normalLitShader.bind();	

	m_normalLitShader.bindUniform("CameraPosition",
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	// Bind the directional light
	m_normalLitShader.bindUniform("LightDirection", m_light.direction);
	// Bind the light color
	m_normalLitShader.bindUniform("LightColor", (glm::vec3)m_light.color);
	// Bind the ambient light using the ambient light
	m_normalLitShader.bindUniform("AmbientColor", m_ambientLight);

	// Bind the diffuse texture location
    m_normalLitShader.bindUniform("diffuseTexture", 0);
	
	// Bind the pvm using the projection view model
	m_normalLitShader.bindUniform("ProjectionViewModel", _pv * _transform);

	// Bind the transform using the model matrix
	m_normalLitShader.bindUniform("ModelMatrix", _transform);

	

	
	/*//m_normalLitShader.bindUniform("normalTexture", 0);
	
	// Bind the texture to a specific location
	m_swordTexture.bind(0);
	m_swordNormalTexture.bind(1);*/

	// Rotates the transform
	_transform = glm::rotate(_transform, m_rotationRate, glm::vec3(0, 1, 0));

	// Draw the bunny
	_mesh->draw();
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
