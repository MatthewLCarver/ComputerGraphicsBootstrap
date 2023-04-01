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
	m_viewMatrix =
		glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix =
		glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);
	
	m_cameraPosition = glm::vec3(glm::inverse(m_viewMatrix)[3]);
	
	m_light.color = glm::vec3(1, 1, 1);
	m_ambientLight = glm::vec3(0.25f, 0.25f, 0.25f);
	
	m_light.direction = glm::vec3(1, -1, 1);
	

	m_orbitalCamera.SetTarget(glm::mat4(1), 10);
	
	m_emitter = new ParticleEmitter();
	m_emitter->Initialise(1000, 500, .1f, 1.0f,
		1, 5, 1, 0.1f,
		glm::vec4(0, 0, 1, 1), glm::vec4(0, 1, 0 ,1));
	
	
	m_scene = new Scene(&m_flyCamera,
		glm::vec2(getWindowWidth(), getWindowHeight()),
		m_light, m_ambientLight);

	m_scene->AddPointLights(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);
	m_scene->AddPointLights(glm::vec3( 0, 5, 0), glm::vec3(0, 0, 0), 1.0f);
	//m_scene->AddPointLights(glm::vec3(3, -5, 0), glm::vec3(0, 0, 1), 500.0f);
	
	
	//m_solarSystem = new SolarSystem();
	
	return LaunchShaders();
}

void GraphicsApp::Shutdown()
{
	Gizmos::destroy();
	delete m_scene;
}


void GraphicsApp::Update(float _deltaTime) {
	m_dt = _deltaTime;
	
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

	for (auto light : m_scene->GetPointLights())
	{
		Gizmos::addSphere(light.direction, 0.1f, 8, 8, glm::vec4(light.color, 1));
	}

	//m_solarSystem->Update(_deltaTime);
	
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	// Grab the time since the application has started
	float time = getTime();

	// use the time to add to the rotation rate variable (in radians)
	m_rotationRate = .002f;
	// Rotate the light to emulate a 'day/night' cycle
	m_light.direction = glm::normalize(glm::vec3(1, glm::sin(time) * 1.5f, glm::cos(time) * 1.5f));
	m_scene->SetGlobalLight(m_light);
	
	m_scene->Update(time);

	// dynamic cast the camera to a base camera
	BaseCamera* camera = m_scene->GetCamera();
	
	
	UpdateCamera(_deltaTime);
	UpdateWeapons();

	m_emitter->Update(_deltaTime, camera->GetTransform(
	camera->GetPosition(), glm::vec3(0), glm::vec3(1))
	);

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	ImGUIRefresher();
}

void GraphicsApp::UpdateCamera(float _deltaTime)
{
	switch(m_scene->GetActiveCameraType())
	{
	case(FLY):
		m_flyCamera.Update(_deltaTime);
		break;

		case(STATIONARY):
			switch(m_currentSCamera)
			{
				case 0:
					m_sCameraX.Update(_deltaTime);
					break;
				case 1:
					m_sCameraY.Update(_deltaTime);
					break;
				case 2:
					m_sCameraZ.Update(_deltaTime);
					break;
			}
			break;

		case(ORBITAL):
			m_orbitalCamera.Update(_deltaTime);
		break;

	default:
		break;
	}
}

void GraphicsApp::UpdateWeapons()
{
	if(m_drawSwords == m_previousDrawSwords)
		return;

	m_previousDrawSwords = m_drawSwords;
	LoadWeaponMesh();
}


void GraphicsApp::draw() {
	// Bind the render target as the first part of our draw function
	m_renderTarget.bind();
	
	// wipe the screen to the background colour
	clearScreen();

	// create simple camera transforms
	CameraTransforms();
	
	auto pv = m_projectionMatrix * m_viewMatrix;
	
	m_scene->Draw();

	m_particleShader.bind();
	m_particleShader.bindUniform("ProjectionViewModel", pv * m_particleEmitTransform);
	m_emitter->Draw();

	//SphereDraw(pv * m_sphereTransform, m_sphereTransform);
	
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// Unbind the render target to return to the back buffer
	m_renderTarget.unbind();

	clearScreen();
	
	//QuadTextureDraw(pv * m_quadTransform);
	//m_scene->Draw();

	// Bind the post process shader and the texture
	m_postProcessShader.bind();
	m_postProcessShader.bindUniform("colorTarget", 0);
	m_postProcessShader.bindUniform("depthTarget", 0);
	m_postProcessShader.bindUniform("windowWidth", (int)getWindowWidth());
	m_postProcessShader.bindUniform("windowHeight", (int)getWindowHeight());
	m_postProcessShader.bindUniform("deltaTime", m_dt);

	m_postProcessShader.bindUniform("lightPosition", m_scene->GetLightPosition());
	m_postProcessShader.bindUniform("eyePosition", m_scene->GetCamera()->GetPosition());
	m_postProcessShader.bindUniform("fogSelector", 2);
	m_postProcessShader.bindUniform("depthFog", 0);
	m_postProcessShader.bindUniform("postProcessTarget", m_postProcessEffect);
	m_renderTarget.getTarget(0).bind(0);

	m_fullscreenQuadMesh.Draw();
	
	/*//QuadDraw(pv * m_quadTransform);
	//IcosahedronDraw(pv * m_icosahedronTransform, m_icosahedronTransform);
	//SpearDraw(pv * m_spearTransform, m_spearTransform);
	//SwordDraw(pv * m_swordTransform, m_swordTransform);

	//BunnyDraw(pv  * m_bunnyTransform);
	//PhongDraw(pv * m_bunnyTransform, m_bunnyTransform);

	
	//CylinderDraw(pv * m_cylinderTransform, m_cylinderTransform);*/

	//m_solarSystem->Draw();
	
	
	
	

}

void GraphicsApp::CameraTransforms()
{
	switch(m_scene->GetActiveCameraType())
	{
	case(FLY):
		m_viewMatrix = m_flyCamera.GetViewMatrix();
		m_projectionMatrix = m_flyCamera.GetProjectionMatrix();
		break;

		case(STATIONARY):

			switch(m_currentSCamera)
			{
				case 0:
					m_viewMatrix = m_sCameraX.GetViewMatrix();
					m_projectionMatrix = m_sCameraX.GetProjectionMatrix();
					break;
				case 1:
					m_viewMatrix = m_sCameraY.GetViewMatrix();
					m_projectionMatrix = m_sCameraY.GetProjectionMatrix();
					break;
				case 2:
					m_viewMatrix = m_sCameraZ.GetViewMatrix();
					m_projectionMatrix = m_sCameraZ.GetProjectionMatrix();
					break;
			}
		
			break;

		case(ORBITAL):
			m_viewMatrix = m_orbitalCamera.GetViewMatrix();
			m_projectionMatrix = m_orbitalCamera.GetProjectionMatrix();
		break;

	default:
		break;
	}
	
	
}

void GraphicsApp::LoadWeaponMesh()
{
	if(m_scene->GetInstances().size() > 0)
	{
		m_scene->ClearInstances();
	}

	switch(m_objectToDraw)
	{
	case SPEARS:
		for(int i = 0; i < 10; i++)
		{
			m_scene->AddInstance(new Instance(glm::vec3 (i * 2, 0.25f, 0),
				glm::vec3(0, i * 30, 0), glm::vec3(1,1,1),
				&m_spearMesh, &m_normalLitShader));
		}
		break;
	case SWORDS:
		for(int i = 0; i < 10; i++)
		{
			m_scene->AddInstance(new Instance(glm::vec3 (i * 2, 3, 0),
				glm::vec3(0, i * 30, 0), glm::vec3(.2f,.2f,.2f),
				&m_swordMesh, &m_normalLitShader));
		}
		break;
	case DRAGONS:
		for(int i = 0; i < 10; i++)
		{
			m_scene->AddInstance(new Instance(glm::vec3 (i * 2, 0.5f, 0),
				glm::vec3(0, i * 30, 0), glm::vec3(.15f,.15f,.15f),
				&m_dragonMesh, &m_litPhongShader));
		}
		break;
	}
}

bool GraphicsApp::LaunchShaders()
{
#pragma region RenderTarget
	if(m_renderTarget.initialise(1, getWindowWidth(), getWindowHeight())
		== false)
	{
		printf("Render Target Error\n");
		return false;
	}
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// Normal Lit Phong Shader
	m_normalLitShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalLit.vert");
	m_normalLitShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalLit.frag");
	
	if (m_normalLitShader.link() == false)
	{
		printf("Normal Lit Phong has an Error: %s\n", m_normalLitShader.getLastError());
		return false;
	}

	// Lit Phong Shader
	m_litPhongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/litPhong.vert");
	m_litPhongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/litPhong.frag");
	
	if (m_litPhongShader.link() == false)
	{
		printf("Lit Phong has an Error: %s\n", m_litPhongShader.getLastError());
		return false;
	}

	// Post Processing Shader
	m_postProcessShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
	m_postProcessShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/post.frag");
	
	if (m_postProcessShader.link() == false)
	{
		printf("Post Process has an Error: %s\n", m_postProcessShader.getLastError());
		return false;
	}

	// Particle Effect Shader
	m_particleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/particle.vert");
	m_particleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/particle.frag");
	
	if (m_particleShader.link() == false)
	{
		printf("Particle Shader has an Error: %s\n", m_particleShader.getLastError());
		return false;
	}

	m_particleEmitTransform = glm::mat4(
		1,  0,  0, 0,
		 0, 1,  0, 0,
		 0,  0, 1, 0,
		 0,  0,  0, 1);

	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	if(m_phongShader.link() == false)
	{
		printf("Standard Phong Shader has an Error: %s\n", m_simpleShader.getLastError());
		return false;
	}
	
	
	if(!QuadTextureLoader())
		return false;
	// Create a full screen quad
	m_fullscreenQuadMesh.InitialiseFullscreenQuad();
	
	/*if(!IcosahedronLoader())
		return false;
	*/
	/*if(!CylinderLoader(10, 1.0f, 0.50f))
		return false;*/
	if(!BoxLoader())
		return false;
	if(!SphereLoader(12))
		return false;
	/*if(!BunnyLoader())
		return false;*/

	
	if(!SpearLoader())
		return false;
	if(!SwordLoader())
		return false;
	if(!DragonLoader())
		return false;

	LoadWeaponMesh();
	
	return true;
}

void GraphicsApp::ImGUIRefresher()
{
	ImGui::Begin("Computer Graphics Test Settings");
	
	ImGui::DragFloat3("Global Light Color", &m_light.color[0], 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Global Light Direction", &m_light.direction[0], 0.01f, -1.0f, 1.0f);
	ImGui::Separator();

	ImGui::DragFloat3("Light 1 Color", &m_scene->GetPointLights()[0].color[0], 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Light 1 Position", &m_scene->GetPointLights()[0].direction[0], 0.1f, -20.0f, 20.0f);	
	ImGui::Separator();

	ImGui::DragFloat3("Light 2 Color", &m_scene->GetPointLights()[1].color[0], 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Light 2 Position", &m_scene->GetPointLights()[1].direction[0], 0.1f, -20.0f, 20.0f);	
	ImGui::Separator();

	if(ImGui::Button("Spears", ImVec2(100, 50)))
	{
		m_objectToDraw = SPEARS;
		LoadWeaponMesh();
	}
	ImGui::SameLine();
	if(ImGui::Button("Swords", ImVec2(100, 50)))
	{
		m_objectToDraw = SWORDS;
		LoadWeaponMesh();
	}
	ImGui::SameLine();
	if(ImGui::Button("Dragons", ImVec2(100, 50)))
	{
		m_objectToDraw = DRAGONS;
		LoadWeaponMesh();
	}
	ImGui::Separator();

	ImGui::Text("Dynamic Cameras");
	if(ImGui::Button("Fly Camera"))
	{
		m_scene->SetCamera(FLY, &m_flyCamera);
	}
	ImGui::SameLine();
	if(ImGui::Button("Orbital Camera"))
	{
		m_scene->SetCamera(ORBITAL, &m_orbitalCamera);
	}
	ImGui::Text("Static Cameras");
	if(ImGui::Button("X Camera"))
	{
		m_currentSCamera = 0;
		m_scene->SetCamera(STATIONARY, &m_sCameraX);
	}
	ImGui::SameLine();
	if(ImGui::Button("Y Camera"))
	{
		m_currentSCamera = 1;
		m_scene->SetCamera(STATIONARY, &m_sCameraY);
	}
	ImGui::SameLine();
	if(ImGui::Button("Z Camera"))
	{
		m_currentSCamera = 2;
		m_scene->SetCamera(STATIONARY, &m_sCameraZ);
	}
	ImGui::Separator();

	if(ImGui::Button("<<", ImVec2(70, 25)) && m_postProcessEffect > -1)
		m_postProcessEffect--;
	ImGui::SameLine();
	ImGui::Text("Post Process Effects");
	ImGui::SameLine();
	if(ImGui::Button(">>", ImVec2(70, 25)) && m_postProcessEffect < 16)
		m_postProcessEffect++;
	ImGui::DragInt(GetPostProcessName(), &m_postProcessEffect, 0.5f, 0, 16);
	
	ImGui::End();
}

const char* GraphicsApp::GetPostProcessName()
{
	switch(m_postProcessEffect)
	{
	case -1:
		return "Default";
	case 0:
		return "Box Blur";
	case 1:
		return "Distortion";
	case 2:
		return "Edge Detection";
	case 3:
		return "Sepia";
	case 4:
		return "Scan Lines";
	case 5:
		return "Grey Scale";
	case 6:
		return "Inverted";
	case 7:
		return "Pixelizer";
	case 8:
		return "Posterizer";
	case 9:
		return "Basic Fog";
	case 10:
		return "Drunk + Glitch";
	case 11:
		return "High Contrast";
	case 12:
		return "Rage";
	case 13:
		return "Depression";
	case 14:
		return "Greed";
	case 15:
		return "Black & White";
	case 16:
		return "Vignette";
	default:
		return "Spear";
	}
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
		1,  0,  0, 0,
		 0, 1,  0, 0,
		 0,  0, 1, 0,
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
		1,  0,  0, 0,
		 0, 1,  0, 0,
		 0,  0, 1, 0,
		 0,  0,  0, 1
	};
	
	return true;
}

void GraphicsApp::BoxDraw(glm::mat4 _pvm, glm::mat4 _transform)
{
	// Bind the Phong shader
	m_litPhongShader.bind();

	m_litPhongShader.bindUniform("CameraPosition", m_cameraPosition);

	// Bind the directional light
	m_litPhongShader.bindUniform("LightDirection", m_light.direction);

	// Bind the pvm using the projection view model
	m_litPhongShader.bindUniform("ProjectionViewModel", _pvm);

	// Bind the transform using the model matrix
	m_litPhongShader.bindUniform("ModelMatrix", _transform);

	// Bind the light color
	m_litPhongShader.bindUniform("LightColor", (glm::vec3)m_light.color);

	// Bind the ambient light using the ambient light
	m_litPhongShader.bindUniform("AmbientColor", m_ambientLight);

	// Bind the ambient, diffuse, specular and specular power
	m_litPhongShader.bindUniform("Ka", glm::vec3(1, 1, 1));
	m_litPhongShader.bindUniform("Kd", glm::vec3(1.0f, 0.4f, 0.4f));
	m_litPhongShader.bindUniform("Ks", glm::vec3(0.1f, 0.1f, 0.1f));
	m_litPhongShader.bindUniform("specularPower", 10);
	
	// Rotates the transform
	//m_boxTransform = glm::rotate(m_boxTransform, m_rotationRate, glm::vec3(0, 1, 0));

	// Draw the quad using the Mesh's draw
	m_boxMesh.Draw();
}

bool GraphicsApp::CylinderLoader(int _segments, float _height, float _radius)
{	
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
	for (int i = 0; i < _segments; ++i)
	{
		indices[_segments * 6 + i * 3 + 0] = _segments * 2;
		indices[_segments * 6 + i * 3 + 1] = i;
		indices[_segments * 6 + i * 3 + 2] = (i + 1) % _segments;
		indices[_segments * 6 + _segments * 3 + i * 3 + 0] = _segments * 2 + 1;
		indices[_segments * 6 + _segments * 3 + i * 3 + 1] = (i + 1) % _segments + _segments;
		indices[_segments * 6 + _segments * 3 + i * 3 + 2] = i + _segments;
	}

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

bool GraphicsApp::SphereLoader(int _sphereSegments)
{
	// Create the vertices and place them using phi and theta
	Mesh::Vertex* vertices = new Mesh::Vertex[_sphereSegments * _sphereSegments];
	for (int i = 0; i < _sphereSegments; ++i)
	{
		float phi = (i / (float)_sphereSegments) * glm::pi<float>() * 2.0f;
		for (int j = 0; j < _sphereSegments; ++j)
		{
			float theta = (j / (float)_sphereSegments) * glm::pi<float>();
			vertices[i * _sphereSegments + j].position = { cos(phi) * sin(theta), cos(theta), sin(phi) * sin(theta), 1 };
		}
	}

	// create the indices
	unsigned int* indices = new unsigned int[_sphereSegments * _sphereSegments * 6];
	for (int i = 0; i < _sphereSegments; ++i)
	{
		for (int j = 0; j < _sphereSegments; ++j)
		{
			indices[(i * _sphereSegments + j) * 6 + 0] = (i * _sphereSegments + j);
			indices[(i * _sphereSegments + j) * 6 + 1] = ((i + 1) % _sphereSegments * _sphereSegments + j);
			indices[(i * _sphereSegments + j) * 6 + 2] = (i * _sphereSegments + (j + 1) % _sphereSegments);
			indices[(i * _sphereSegments + j) * 6 + 3] = ((i + 1) % _sphereSegments * _sphereSegments + j);
			indices[(i * _sphereSegments + j) * 6 + 4] = ((i + 1) % _sphereSegments * _sphereSegments + (j + 1) % _sphereSegments);
			indices[(i * _sphereSegments + j) * 6 + 5] = (i * _sphereSegments + (j + 1) % _sphereSegments);
		}
	}

	// initialise the mesh
	m_sphereMesh.Initialise(_sphereSegments * _sphereSegments, vertices, _sphereSegments * _sphereSegments * 6, indices);
	m_sphereTransform = {
		1,  0,  0, 0,
		0, 1,  0, 0,
		0,  0, 1, 0,
		0,  0,  0, 1
	};
	
	return true;
}

void GraphicsApp::SphereDraw(glm::mat4 _pv, glm::mat4 _transform)
{		
	// Bind the Phong shader
	m_litPhongShader.bind();

	m_litPhongShader.bindUniform("CameraPosition", m_cameraPosition);

	// Bind the directional light
	m_litPhongShader.bindUniform("LightDirection", m_light.direction);

	// Bind the pvm using the projection view model
	m_litPhongShader.bindUniform("ProjectionViewModel", _pv * _transform);

	// Bind the transform using the model matrix
	m_litPhongShader.bindUniform("ModelMatrix", _transform);

	// Bind the light color
	m_litPhongShader.bindUniform("LightColor", (glm::vec3)m_light.color);

	// Bind the ambient light using the ambient light
	m_litPhongShader.bindUniform("AmbientColor", m_ambientLight);

	// Bind the ambient, diffuse, specular and specular power
	m_litPhongShader.bindUniform("Ka", glm::vec3(1.0f, 0.0f, 0.0f));
	m_litPhongShader.bindUniform("Kd", glm::vec3(1.0f, 0.4f, 0.4f));
	m_litPhongShader.bindUniform("Ks", glm::vec3(0.1f, 0.1f, 0.1f));
	m_litPhongShader.bindUniform("specularPower", 10);

	// Rotates the transform
	//m_sphereTransform = glm::rotate(m_sphereTransform, m_rotationRate, glm::vec3(0, 1, 0));

	// Draw the quad using the Mesh's draw
	m_sphereMesh.Draw();
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
	if(m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soul Spear Mesh Error!\n");
		return false;
	}

	m_spearTransform = {
		1,  0,  0, 0,
		0,  1,  0, 0,
		0,  0,  1, 0,
		0,  0,  0, 1
	};

	return true;
}

/*void GraphicsApp::SpearDraw(glm::mat4 _pvm, glm::mat4 _transform)
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
}*/

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

	//m_swordTransform = glm::translate(m_swordTransform, glm::vec3(15.0f, 13.0f, 0.0f));

	return true;
}

/*void GraphicsApp::SwordDraw(glm::mat4 _pvm, glm::mat4 _transform)
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

	// Draw the sword
	m_swordMesh.draw();
}*/

bool GraphicsApp::DragonLoader()
{
	if(m_dragonMesh.load("./stanford/Dragon.obj", true, true) == false)
	{
		printf("Dragon Mesh Error!\n");
		return false;
	}

	m_dragonTransform = {
		0.75f,  0,  0, 0,
		0,  0.75f,  0, 0,
		0,  0,  0.75f, 0,
		0,  0,  0, 1
	};

	return true;
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
	//m_gridTexture.bind(0);
	m_renderTarget.getTarget(0).bind(0);

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
	
	// Rotates the transform
	_transform = glm::rotate(_transform, m_rotationRate, glm::vec3(0, 1, 0));

	// Draw the mesh
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
