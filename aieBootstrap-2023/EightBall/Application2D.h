#pragma once

#include <Input.h>

#include "Application.h"
#include "Renderer2D.h"
#include <glm/vec2.hpp>
#include <vector>

#include "PoolBall.h"

class PoolBall;
class PhysicsScene;
class PhysicsObject;
class Circle;

struct Player
{
	int m_score = 0;
	BallType m_ballType;

	void IncrementScore() { m_score++; }
	
	void SetBallType(BallType _ballType) { m_ballType = _ballType; }
	BallType GetBallType() { return m_ballType; }
};

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool Startup();
	virtual void Shutdown();

	
	virtual void Update(float deltaTime);
	virtual void draw();


protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_texture;
	aie::Texture*		m_shipTexture;
	aie::Font*			m_font;

	float m_timer;

	float GetExtents() { return m_extents; }
	float GetAspect() { return m_aspectRatio; }

private:
	PhysicsScene* m_physicsScene;

	aie::Texture* m_backgroundTexture = nullptr;
	aie::Texture* m_tableTexture = nullptr;

	void CreatePlayers();
	// Init Functions
	void PoolSetup();
	void SetupTableAndPockets();
	void SetupPocketTriggers();
	void AddToBallList();
	void SetupBalls();

	
	// Update Functions
	void HandleGameplay(aie::Input* _input, float _deltaTime);
	bool CheckVelocities(float _deltaTime);
	void OnPoolBallSunkTrigger(PoolBall* _other);
	void SetFirstBallTypeToPlayer(BallType ball);
	void CheckPlayerAgainstBallType(PoolBall* _ball);
	void UpdatePlayerBallType();
	void AssessPlayerFirstBallHit();
	
	void RemoveWhiteBall();
	void PlaceWhiteBall();
	void GameOver(Player* player);

	void ProcessTurn();
	Player* GetPlayer(PoolBall* _ball);
	
	void DisplayBallIOnPlayerScoreboard(PoolBall* _ball);
	void DisplayGameOver() {};
	
	// Helper Functions
	glm::vec2 ScreenToWorld(glm::vec2 _screenPos);
	glm::vec2 WorldToScreen(glm::vec2 _worldPos);
	float DegreeToRadian(float _degree);
	float RadianToDegree(float _radian);
	float GetAngleBetweenPoints(glm::vec2 _point1, glm::vec2 _point2);
	float GetLengthBetweenPoints(glm::vec2 _point1, glm::vec2 _point2);
	glm::vec2 CalculateForce(glm::vec2 _point1, glm::vec2 _point2);

	const float m_extents = 250;
	const float m_aspectRatio = 16.0f / 9.0f;
	float m_velocityTimer = 0;
	float m_turnTimer = 7;

	glm::vec2 m_mousePos = glm::vec2(0);
	glm::vec2 m_previousTurnVelocity = glm::vec2(0);

	float m_poolBallRadius = 5;
	float m_poolBallMass = 3;

	PoolBall* m_whiteBall = nullptr;
	PoolBall* m_blackBall = nullptr;
	std::vector<PoolBall*> m_solidBalls;
	std::vector<PoolBall*> m_stripedBalls;
	std::vector<PoolBall*> m_allBalls;
	PoolBall* m_firstBallHit = nullptr;
	
	aie::Texture* m_whiteBallTexture;
	aie::Texture* m_deploymentZoneTexture;
	
	std::vector<Circle*> m_pockets;
	Player* m_playerOne = nullptr;
	Player* m_playerTwo = nullptr;
	Player* m_winner = nullptr;

	int m_turnCount = 1;
	
	// Bools
	bool m_isShooting = false;
	bool m_isFoul = false;
	bool m_isPlayerOneTurn = true;
	bool m_hasLostGame = false;
	bool m_turnProcessed = true;
	bool m_hasClicked = false;
	bool m_whiteBallReset = true;
};
