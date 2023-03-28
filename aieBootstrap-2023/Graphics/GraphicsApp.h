#pragma once

#include "Application.h"
#include "FlyCamera.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Shader.h"
#include "SimpleCamera.h"
#include "Scene.h"
#include "Instance.h"
#include "RenderTarget.h"
#include "ParticleEmitter.h"

#include <imgui.h>
#include <glm/mat4x4.hpp>


#include "OrbitalCamera.h"
#include "SolarSystem.h"
#include "StationaryCamera.h"

enum ObjectToDraw
{
	SPEARS,
	SWORDS,
	DRAGONS
};

class GraphicsApp : public aie::Application {
public:

	GraphicsApp();
	virtual ~GraphicsApp();
	
	virtual bool Startup();
	virtual void Shutdown();

	void UpdateCamera(float _deltaTime);
	void UpdateWeapons();
	
	virtual void Update(float _deltaTime);
	
	virtual void draw();

	void CameraTransforms();
	
protected:
	void LoadWeaponMesh();
	bool LaunchShaders();
	void ImGUIRefresher();
	
	bool QuadLoader();
	void QuadDraw(glm::mat4 _pvm);

	bool BoxLoader();
	void BoxDraw(glm::mat4 _pvm, glm::mat4 _transform);

	bool CylinderLoader(int _segments, float _height, float _radius);
	void CylinderDraw(glm::mat4 _pvm, glm::mat4 _transform);

	bool PyramidLoader();
	void PyramidDraw(glm::mat4 _pvm);

	bool SphereLoader();
	void SphereDraw(glm::mat4 _pvm);

	bool CapsuleLoader();
	void CapsuleDraw(glm::mat4 _pvm);
	
	bool ConeLoader();
	void ConeDraw(glm::mat4 _pvm);

	bool GridLoader();
	void GridDraw(glm::mat4 _pvm);

	bool TorusLoader();
	void TorusDraw(glm::mat4 _pvm);

	bool IcosahedronLoader();
	void IcosahedronDraw(glm::mat4 _pvm, glm::mat4 _transform);
	
	
	bool BunnyLoader();
	void BunnyDraw(glm::mat4 _pvm);

	bool SpearLoader();
	void SpearDraw(glm::mat4 _pvm, glm::mat4 _transform);

	bool SwordLoader();
	void SwordDraw(glm::mat4 _pvm, glm::mat4 _transform);

	bool DragonLoader();

	bool QuadTextureLoader();
	void QuadTextureDraw(glm::mat4 _pvm);

	// For Textured OBJs
	void ObjDraw(glm::mat4 _pv,
		glm::mat4 _transform, aie::OBJMesh* _mesh);
	
	void PhongDraw(glm::mat4 _pvm, glm::mat4 _transform);

	int m_postProcessEffect = -1;

	Scene*				m_scene;
	
	// camera transforms
	glm::mat4			m_viewMatrix;
	glm::mat4			m_projectionMatrix;

	aie::Texture		m_gridTexture;
	aie::Texture		m_spearTexture;
	
	aie::Texture		m_swordTexture;
	aie::Texture		m_swordNormalTexture;
	aie::Texture		m_swordSpecularTexture;
	aie::Texture		m_swordAoTexture;
	
	aie::ShaderProgram	m_simpleShader;
	aie::ShaderProgram	m_colorShader;
	aie::ShaderProgram	m_phongShader;
	aie::ShaderProgram	m_litPhongShader;
	aie::ShaderProgram	m_texturedShader;
	aie::ShaderProgram	m_normalLitShader;
	aie::ShaderProgram	m_postProcessShader;
	aie::ShaderProgram	m_particleShader;

	aie::RenderTarget	m_renderTarget;
	
	Mesh				m_quadMesh;
	Mesh				m_fullscreenQuadMesh;
	glm::mat4			m_quadTransform;

	Mesh				m_boxMesh;
	glm::mat4			m_boxTransform;

	Mesh				m_cylinderMesh;
	glm::mat4			m_cylinderTransform;

	Mesh				m_pyramidMesh;
	glm::mat4			m_pyramidTransform;

	Mesh				m_icosahedronMesh;
	glm::mat4			m_icosahedronTransform;

	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;

	aie::OBJMesh		m_spearMesh;
	glm::mat4			m_spearTransform;

	aie::OBJMesh		m_swordMesh;
	glm::mat4			m_swordTransform;

	aie::OBJMesh		m_dragonMesh;
	glm::mat4			m_dragonTransform;

	aie::OBJMesh		m_r2D2Mesh;
	glm::mat4			m_r2D2Transform;

	SimpleCamera		m_camera;
	FlyCamera			m_flyCamera;
	StationaryCamera	m_sCameraX = StationaryCamera(glm::vec3(0, 1, -20), glm::vec3(90, 0, 0));
	StationaryCamera	m_sCameraY = StationaryCamera(glm::vec3(0, 30, 0), glm::vec3(0, 270, 0));
	StationaryCamera	m_sCameraZ = StationaryCamera(glm::vec3(-10, 1, 0), glm::vec3(0, 0, 0));
	OrbitalCamera		m_orbitalCamera;
	
	Light				m_light;
	glm::vec3			m_ambientLight;

	glm::vec3			m_cameraPosition;

	float				m_rotationRate = 0.0f;

	ParticleEmitter*	m_emitter;
	glm::mat4			m_particleEmitTransform;
	

private:
	SolarSystem* m_solarSystem = nullptr;
	bool m_drawSwords = false;
	bool m_previousDrawSwords = false;
	int m_currentSCamera = 0;

	ObjectToDraw m_objectToDraw = SPEARS;
	float m_dt;
};