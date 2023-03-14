#include "Application2D.h"

#include <Demos.h>
#include <iostream>
#include <string>
#include "Box.h"
#include "Circle.h"
//#include "Plane.h""

#include <Gizmos.h>
#include "Font.h"
#include "Input.h"
#include "PhysicsObject.h"
#include "PhysicsScene.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include "PoolBall.h"

Application2D::Application2D()
{
	
}

Application2D::~Application2D()
{
	delete m_backgroundTexture;
	delete m_tableTexture;
	delete m_whiteBallTexture;
	delete m_deploymentZoneTexture;
}

bool Application2D::Startup() {

	//Increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);

	m_physicsScene = new PhysicsScene();
	
	m_2dRenderer = new aie::Renderer2D();

	m_texture = new aie::Texture("./textures/numbered_grid.tga");
	m_shipTexture = new aie::Texture("./textures/ship.png");

	// Setup pool textures
	m_backgroundTexture = new aie::Texture("./textures/Pool/Floor.png");
	m_tableTexture = new aie::Texture("./textures/Pool/Table.png");
	m_whiteBallTexture = new aie::Texture("./textures/Pool/ball_16.png");
	m_deploymentZoneTexture = new aie::Texture("./textures/Pool/DeploymentZone.png");

	PoolSetup();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_timer = 0;

	return true;
}

void Application2D::Shutdown() {
	
	delete m_font;
	delete m_texture;
	delete m_shipTexture;
	delete m_2dRenderer;
}

void Application2D::Update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	//m_physicsScene->Draw();

	int xScreen, yScreen;
	input->getMouseXY(&xScreen, &yScreen);
	m_mousePos = ScreenToWorld(glm::vec2(xScreen, yScreen));
	/*// Update the camera position using the arrow keys
	float camPosX;
	float camPosY;
	m_2dRenderer->getCameraPos(camPosX, camPosY);

	if (input->isKeyDown(aie::INPUT_KEY_UP))
		camPosY += 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		camPosY -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		camPosX -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		camPosX += 500.0f * deltaTime;

	m_2dRenderer->setCameraPos(camPosX, camPosY);*/

	if(!m_hasLostGame)
	{
		HandleGameplay(input, deltaTime);
	}
	else
	{
		//DisplayGameOver();
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio,
		m_extents / m_aspectRatio,
		-1.0f, 1.0f));
	
	
	// 2d tiled array background
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			m_2dRenderer->drawSprite(m_backgroundTexture,
				(float)(x * (m_backgroundTexture->getWidth() * 2)),
				(float)(y * (m_backgroundTexture->getHeight() * 2)),
				(float)m_backgroundTexture->getWidth() * 2,
				(float)m_backgroundTexture->getHeight() * 2,
				0, 1);
		}
	}
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			m_2dRenderer->drawSprite(m_backgroundTexture,
				(float)(x * (m_backgroundTexture->getWidth() * 2)),
				(float)(y * (m_backgroundTexture->getHeight() * 2)),
				(float)m_backgroundTexture->getWidth() * 2,
				(float)m_backgroundTexture->getHeight() * 2,
				0, 1);
		}
	}

	// Table
	m_2dRenderer->drawSprite(m_tableTexture,
		(float)getWindowWidth() / 2, (float)getWindowHeight() / 2,
		(float)(getWindowWidth()) - ((float)getWindowWidth() /4), (float)(getWindowHeight()) - ((float)getWindowHeight() / 4),
		0, 0.5f,
		0.5f, 0.5f);

	// Balls
	for(int i = 0; i < m_solidBalls.size(); i++)
	{
		//if(!m_solidBalls[i]->HasSunk())
			m_2dRenderer->drawSprite(
				m_solidBalls[i]->GetBallTexture(),
				WorldToScreen(m_solidBalls[i]->GetPosition()).x,
				WorldToScreen(m_solidBalls[i]->GetPosition()).y,
				m_solidBalls[i]->GetRadius() * 8, m_solidBalls[i]->GetRadius() * 8,
				m_solidBalls[i]->GetOrientation(), 0, 0.5f, 0.5f
			);
	}
	for(int i = 0; i < m_stripedBalls.size(); i++)
	{
		//if(!m_stripedBalls[i]->HasSunk())
			m_2dRenderer->drawSprite(
				m_stripedBalls[i]->GetBallTexture(), 
				WorldToScreen(m_stripedBalls[i]->GetPosition()).x,
				WorldToScreen(m_stripedBalls[i]->GetPosition()).y,
				m_stripedBalls[i]->GetRadius() * 8, m_stripedBalls[i]->GetRadius() * 8,
				m_stripedBalls[i]->GetOrientation(), 0, 0.5f, 0.5f
			);
	}

	if(!m_whiteBall->HasSunk())
		m_2dRenderer->drawSprite(
			m_whiteBall->GetBallTexture(),
			WorldToScreen(m_whiteBall->GetPosition()).x,
			WorldToScreen(m_whiteBall->GetPosition()).y,
			m_whiteBall->GetRadius() * 8, m_whiteBall->GetRadius() * 8,
			m_whiteBall->GetOrientation(), 0, 0.5f, 0.5f
		);

	if(!m_blackBall->HasSunk())
	m_2dRenderer->drawSprite(
		m_blackBall->GetBallTexture(),
		WorldToScreen(m_blackBall->GetPosition()).x,
		WorldToScreen(m_blackBall->GetPosition()).y,
		m_blackBall->GetRadius() * 8, m_blackBall->GetRadius() * 8,
		m_blackBall->GetOrientation(), 0, 0.5f, 0.5f
	);

	m_2dRenderer->setRenderColour(0.5f, 0.2f, 0.2f, 0.9f);
	// Draw box Cue stick
	if (!m_isShooting && !m_whiteBall->HasSunk() && m_turnProcessed)
		m_2dRenderer->drawLine(
			WorldToScreen(m_mousePos).x, WorldToScreen(m_mousePos).y,
			WorldToScreen(m_whiteBall->GetPosition()).x, WorldToScreen(m_whiteBall->GetPosition()).y,
			10, .2f
		);

	m_2dRenderer->setRenderColour(1, 1, 1, 1);
	// UI - uses the last used colour
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 1600, 50);
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 1600, 20);

	if(m_isPlayerOneTurn)
	{
		m_2dRenderer->drawText(m_font, "Player 1's turn", 800, 75);
		if(m_playerOne->GetBallType() ==  0)
			m_2dRenderer->drawText(m_font, "Targeting Any", 820, 30);
		else if (m_playerOne->GetBallType() ==  SOLID)
			m_2dRenderer->drawText(m_font, "Targeting Solid", 800, 30);
		else if (m_playerOne->GetBallType() ==  STRIPED)
			m_2dRenderer->drawText(m_font, "Targeting Striped", 800, 30);
		else
			m_2dRenderer->drawText(m_font, "Targeting Black", 820, 30);
	}
	else
	{
		m_2dRenderer->drawText(m_font, "Player 2's turn", 800, 75);
		if(m_playerTwo->GetBallType() ==  0)
			m_2dRenderer->drawText(m_font, "Targeting Any", 820, 30);
		else if (m_playerTwo->GetBallType() ==  SOLID)
			m_2dRenderer->drawText(m_font, "Targeting Solid", 800, 30);
		else if (m_playerTwo->GetBallType() ==  STRIPED)
			m_2dRenderer->drawText(m_font, "Targeting Striped", 790, 30);
		else
			m_2dRenderer->drawText(m_font, "Targeting Black", 780, 30);
	}

	// Scoreboard UI
	m_2dRenderer->drawText(m_font, "Solids", 25, 1000);
	m_2dRenderer->drawText(m_font, "______________________________", 25, 1000);
	m_2dRenderer->drawText(m_font, "Striped", 1775, 1000);
	m_2dRenderer->drawText(m_font, "________________________________", 1335, 1000);

	if(m_whiteBall->HasSunk())
	{
		m_2dRenderer->drawSprite(m_deploymentZoneTexture, 1425, getWindowHeight()/2,
			279 - (m_poolBallRadius * 8), 626 - (m_poolBallRadius * 10),
			0, 0, 0.5f, 0.5f);
		
		glm::vec2 mousePos = WorldToScreen(m_mousePos);
		m_2dRenderer->drawSprite(m_whiteBallTexture, mousePos.x, mousePos.y, m_whiteBall->GetRadius() * 8, m_whiteBall->GetRadius() * 8, 0, 0, 0.5f, 0.5f);
	}

	if(m_isFoul)
		m_2dRenderer->drawText(m_font, "FOUL!", 910, 1000);

	if(m_winner != nullptr)
	{
		if(m_winner == m_playerOne)
			m_2dRenderer->drawText(m_font, "Player 1 Wins!", (getWindowWidth() / 2) - 50, getWindowHeight() / 2);
		else
			m_2dRenderer->drawText(m_font, "Player 2 Wins!", (getWindowWidth() / 2) - 50, getWindowHeight() / 2);
	
		m_2dRenderer->drawText(m_font, "GAME OVER!", (getWindowWidth() / 2) - 50, (getWindowHeight() / 2) + 100);
	}

	if (m_turnCount > 1 && !m_isFoul)
		m_2dRenderer->drawText(m_font, "Two turns!", 880, 1000);

	m_2dRenderer->drawText(m_font, "LMB: Hit the white ball", 25, 80);
	m_2dRenderer->drawText(m_font, "     Click further away for more power", 25, 50);
	m_2dRenderer->drawText(m_font, "RMB: Place the white ball", 25, 20);


	// done drawing sprites
	m_2dRenderer->end();
}

void Application2D::CreatePlayers()
{
	m_playerOne = new Player();
	m_playerTwo = new Player();
}

void Application2D::PoolSetup()
{
	CreatePlayers();
	SetupTableAndPockets();
	SetupBalls();
	SetupPocketTriggers();
}

void Application2D::SetupTableAndPockets()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Ends of the table
	m_physicsScene->AddActor(new Box(glm::vec2(171.5f, 0), glm::vec2(0), 0,
		4, glm::vec2(8, 69), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(-171.5f, 0), glm::vec2(0), 0,
		4, glm::vec2(8, 69), glm::vec4(0, .4f, .1f, 1), true));
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Sides of the table
	m_physicsScene->AddActor(new Box(glm::vec2(80, 89.5f), glm::vec2(0), 0,
		4, glm::vec2(70, 8), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(-83.5f, 89.5f), glm::vec2(0), 0,
		4, glm::vec2(69.5, 8), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(80, -89.5f), glm::vec2(0), 0,
		4, glm::vec2(70, 8), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(-83.5f, -89.5f), glm::vec2(0), 0,
		4, glm::vec2(69.5, 8), glm::vec4(0, .4f, .1f, 1), true));
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Angled Side of the table
	// Top Left
	m_physicsScene->AddActor(new Box(glm::vec2(-166.5f, 72), glm::vec2(0), DegreeToRadian(45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(-156.5f, 85), glm::vec2(0), DegreeToRadian(45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	// Top Right
	m_physicsScene->AddActor(new Box(glm::vec2(166.5f, 72), glm::vec2(0), DegreeToRadian(-45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(153.5f, 85), glm::vec2(0), DegreeToRadian(-45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	// Bottom Left
	m_physicsScene->AddActor(new Box(glm::vec2(-166.5f, -72), glm::vec2(0), DegreeToRadian(-45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(-156.5f, -85), glm::vec2(0), DegreeToRadian(-45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	// Bottom Right
	m_physicsScene->AddActor(new Box(glm::vec2(166.5f, -72), glm::vec2(0), DegreeToRadian(45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(153.5f, -85), glm::vec2(0), DegreeToRadian(45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Top Centre
	m_physicsScene->AddActor(new Box(glm::vec2(-12, 85), glm::vec2(0), DegreeToRadian(-25),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(8, 85), glm::vec2(0), DegreeToRadian(25),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	// Bottom Centre
	m_physicsScene->AddActor(new Box(glm::vec2(-12, -85), glm::vec2(0), DegreeToRadian(25),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(8, -85), glm::vec2(0), DegreeToRadian(-25),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pockets
	Circle* topRight = new Circle(glm::vec2(170, 88), glm::vec2(0), 0, 7,
		glm::vec4(0, .1f, 0, 1), true, true);
	Circle* topLeft = new Circle(glm::vec2(-172, 88), glm::vec2(0), 0, 7,
		glm::vec4(0, .1f, 0, 1), true, true);
	Circle* bottomRight = new Circle(glm::vec2(170, -88), glm::vec2(0), 0, 7,
		glm::vec4(0, .1f, 0, 1), true, true);
	Circle* bottomLeft = new Circle(glm::vec2(-172, -88), glm::vec2(0), 0, 7,
		glm::vec4(0, .1f, 0, 1), true, true);
	Circle* topCentre = new Circle(glm::vec2(-2, 92.5f), glm::vec2(0), 0, 6,
		glm::vec4(0, .1f, 0, 1), true, true);
	Circle* bottomCentre = new Circle(glm::vec2(-2, -92.5f), glm::vec2(0), 0, 6,
		glm::vec4(0, .1f, 0, 1), true, true);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_physicsScene->AddActor(topLeft);
	m_physicsScene->AddActor(topCentre);
	m_physicsScene->AddActor(topRight);
	m_physicsScene->AddActor(bottomRight);
	m_physicsScene->AddActor(bottomCentre);
	m_physicsScene->AddActor(bottomLeft);

	m_pockets.push_back(topLeft);
	m_pockets.push_back(topCentre);
	m_pockets.push_back(topRight);
	m_pockets.push_back(bottomRight);
	m_pockets.push_back(bottomCentre);
	m_pockets.push_back(bottomLeft);
}

void Application2D::SetupPocketTriggers()
{
	for (auto& m_pocket : m_pockets)
	{
		m_pocket->TriggerEnterCallback = [=] (PhysicsObject* _other)
		{
			PoolBall* ball = dynamic_cast<PoolBall*>(_other);
			OnPoolBallSunkTrigger(ball);
		};
	}
}

void Application2D::AddToBallList()
{
	// Loops through all the balls in play and add them to a single list for velocity checking
	
	for (int i = 0; i < 7; i++)
	{
		m_allBalls.push_back(m_solidBalls[i]);
		m_allBalls.push_back(m_stripedBalls[i]);
	}
	
	m_allBalls.push_back(m_blackBall);
	m_allBalls.push_back(m_whiteBall);
}

void Application2D::SetupBalls()
{
	// Setup the ball textures
	std::vector<aie::Texture*> m_ballTex = std::vector<aie::Texture*>(16); 
	for( int i = 1; i < 17; i++)
	{
		m_ballTex[i-1] = new aie::Texture(("./textures/Pool/ball_" + std::to_string(i) + ".png").c_str());
	}

	m_whiteBall = new PoolBall(
		m_ballTex[15], // Texture
		WHITE, // Ball Type
		glm::vec2(91, 0), // position
		glm::vec2(0), // velocity
		m_poolBallMass, // mass
		m_poolBallRadius, // radius
		glm::vec4(1, 1, 1, 1), // color
		false, false); // kinematic, trigger
	m_physicsScene->AddActor(m_whiteBall);

	// Place the balls into a triangle on the table
	int ballsPlaced = 0;
	float heightAdjuster = 0;
	float poolDiameter = m_poolBallRadius * 2;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 1 + i; j++)
		{
			int mid = (1 + i) / 2;
			if(mid == j)
			{
				heightAdjuster = 0;
			}
			else if(mid > j)
			{
				heightAdjuster = m_poolBallRadius + (mid - j) * m_poolBallRadius;
			}
			else
			{
				heightAdjuster = -m_poolBallRadius - (j - mid) * m_poolBallRadius;
			}
			
			if(ballsPlaced == 4)
			{
				// push back the 8 ball in next position
				m_blackBall = new PoolBall(
					m_ballTex[7], // Texture
					BLACK, // Ball Type
						glm::vec2( // position
						-60 - (i * poolDiameter),
						0 + heightAdjuster),
					glm::vec2(0), // velocity
					m_poolBallMass, // mass
					m_poolBallRadius, // radius
					glm::vec4(1, 1, 1, 1), // color
					false, false); // kinematic, trigger
				m_physicsScene->AddActor(m_blackBall);
				ballsPlaced++;
				continue;
			}
			if(i % 2 != 0)
			{
				heightAdjuster -= m_poolBallRadius;
			}
			if (ballsPlaced % 2 != 1)
			{
				m_solidBalls.push_back(new PoolBall(
					m_ballTex[m_solidBalls.size()], // Texture
					SOLID, // Ball Type
						glm::vec2( // position
						-60 - (i * poolDiameter),
						0 + heightAdjuster),
					glm::vec2(0), // velocity
					m_poolBallMass, // mass
					m_poolBallRadius, // radius
					glm::vec4(1, 1, 1, 1), // color
					false, false)); // kinematic, trigger
				m_physicsScene->AddActor(m_solidBalls[m_solidBalls.size()-1]);
			}
			else
			{
				m_stripedBalls.push_back(new PoolBall(
					m_ballTex[m_stripedBalls.size() + 8], // Texture
					STRIPED, // Ball Type
						glm::vec2( // position
						-60 - (i * poolDiameter),
						0 + heightAdjuster),
					glm::vec2(0), // velocity
					m_poolBallMass, // mass
					m_poolBallRadius, // radius
					glm::vec4(1, 1, 1, 1), // color
					false, false)); // kinematic, trigger
				m_physicsScene->AddActor(m_stripedBalls[m_stripedBalls.size()-1]);
			}
			
			//m_physicsScene->AddActor(m_solidBalls[i]);
			ballsPlaced++;
		}
	}

	// Add all the balls to a single list
	AddToBallList();
}

//Trigger function
void Application2D::OnPoolBallSunkTrigger(PoolBall* _ball)
{
	switch((int)_ball->GetBallType())
	{
	case (int)WHITE:
		//std::cout << "WHITE BALL SUNK" << std::endl;
		m_whiteBall->SinkBall();
		RemoveWhiteBall();
		m_isFoul = true;
		break;
		
	case (int)BLACK:
		//std::cout << "BLACK BALL SUNK" << std::endl;
		CheckPlayerAgainstBallType(_ball);
		break;
		
	case (int)SOLID:
		//std::cout << "SOLID BALL SUNK" << std::endl;
		SetFirstBallTypeToPlayer(_ball->GetBallType());
		CheckPlayerAgainstBallType(_ball);
		break;
		
	case (int)STRIPED:
		//std::cout << "STRIPED BALL SUNK" << std::endl;
		SetFirstBallTypeToPlayer(_ball->GetBallType());
		CheckPlayerAgainstBallType(_ball);
		break;

	default:
		//std::cout << "BROKEN BALL SUNK" << std::endl;
		break;
	}
}

void Application2D::SetFirstBallTypeToPlayer(BallType ball)
{
	switch (m_isPlayerOneTurn)
	{
		case true:
			if(m_playerOne->GetBallType() == 0)
			{
				if(ball == SOLID)
				{
					m_playerOne->SetBallType(SOLID);
					m_playerTwo->SetBallType(STRIPED);
				}
				else
				{
					m_playerOne->SetBallType(STRIPED);
					m_playerTwo->SetBallType(SOLID);
				}
			}
			break;
		case false:
			if(m_playerTwo->GetBallType() == 0)
			{
				if(ball == SOLID)
				{
					m_playerOne->SetBallType(STRIPED);
					m_playerTwo->SetBallType(SOLID);
				}
				else
				{
					m_playerOne->SetBallType(SOLID);
					m_playerTwo->SetBallType(STRIPED);
				}
			}
	}
}

void Application2D::CheckPlayerAgainstBallType(PoolBall* _ball)
{
	switch (m_isPlayerOneTurn)
	{
		case true:
			if(_ball->GetBallType() == BLACK)
			{
				if(m_playerOne->GetBallType() == _ball->GetBallType())
				{
					GameOver(m_playerOne);
				}
				else
				{
					GameOver(m_playerTwo);
					m_hasLostGame = true;
				}
				break;
			}
		
			if(m_playerOne->GetBallType() != _ball->GetBallType())
			{
				m_isFoul = true;
				m_playerTwo->IncrementScore();
			}
			else
			{
				m_playerOne->IncrementScore();
				m_turnCount++;
			}
			break;
		
		case false:
			if(_ball->GetBallType() == BLACK)
			{
				if(m_playerTwo->GetBallType() == _ball->GetBallType())
				{
					GameOver(m_playerTwo);
				}
				else
				{
					GameOver(m_playerOne);
					m_hasLostGame = true;
				}
				break;
			}
		
			if(m_playerTwo->GetBallType() != _ball->GetBallType())
			{
				m_isFoul = true;
				m_playerOne->IncrementScore();
			}
			else
			{
				m_playerTwo->IncrementScore();
				m_turnCount++;
			}
			break;
	}

	DisplayBallIOnPlayerScoreboard(_ball);
	UpdatePlayerBallType();
}

void Application2D::UpdatePlayerBallType()
{
	if(m_playerOne->m_score >= 7)
	{
		m_playerOne->SetBallType(BLACK);
	}
	if(m_playerTwo->m_score >= 7)
	{
		m_playerTwo->SetBallType(BLACK);
	}

}

void Application2D::AssessPlayerFirstBallHit()
{
	if(m_firstBallHit == nullptr || m_firstBallHit->GetBallType() == WHITE)
		return;

	if (m_firstBallHit->GetBallType() == BLACK)
		m_isFoul = true;

	if(m_isPlayerOneTurn)
	{
		if(m_firstBallHit->GetBallType() != m_playerOne->GetBallType() &&
			m_playerOne->GetBallType() != WHITE)
		{
			m_isFoul = true;
		}
	}
	else
	{
		if(m_firstBallHit->GetBallType() != m_playerTwo->GetBallType() &&
			m_playerTwo->GetBallType() != WHITE)
		{
			m_isFoul = true;
		}
	}
}

void Application2D::RemoveWhiteBall()
{
	m_whiteBall->SetVelocity(glm::vec2(0));
	m_whiteBall->SetAngularVelocity(0);
	m_whiteBall->SetOrientation(0);
	m_whiteBall->SetPosition(glm::vec2(2000, 2000));
}

void Application2D::PlaceWhiteBall()
{
	//std::cout << "x = " << m_mousePos.x << std::endl << " y = " << m_mousePos.y << std::endl;
	if(!(m_mousePos.x < 150 && m_mousePos.x > 90 &&
		m_mousePos.y < 75 && m_mousePos.y > -75))
		return;
	
	m_whiteBall->SinkBall();
	m_whiteBall->SetPosition(m_mousePos);
	m_whiteBallReset = true;
}

void Application2D::GameOver(Player* player)
{
	m_winner = player;
}

void Application2D::ProcessTurn()
{
	if (m_firstBallHit == nullptr)
		m_isFoul = true;

	if(m_isFoul)
 	{
 		m_isPlayerOneTurn = !m_isPlayerOneTurn;
 		m_turnCount = 2;
 		m_isFoul = false;
		m_turnProcessed = true;
		return;
 	}
	
	m_turnCount--;
	if(m_turnCount == 0)
	{
		m_isPlayerOneTurn = !m_isPlayerOneTurn;
		m_turnCount = 1;
	}

	m_turnProcessed = true;
}

Player* Application2D::GetPlayer(PoolBall* _ball)
{
	if(m_playerOne->GetBallType() == _ball->GetBallType())
	{
		return m_playerOne;
	}
	return m_playerTwo;
}

void Application2D::DisplayBallIOnPlayerScoreboard(PoolBall* _ball)
{
	_ball->SetVelocity(glm::vec2(0));
	_ball->SetOrientation(0);
	_ball->SetAngularVelocity(0);

	
	int score = GetPlayer(_ball)->m_score;
	
	if(_ball->GetBallType() == SOLID)
	{
		_ball->SetPosition(glm::vec2(-220 + ((score) * 15), 125));
	}
	else
	{
		//m_sunkStripedBallTextures.push_back();
		_ball->SetPosition(glm::vec2( 210 - ((score) * 15), 125));
	}
	_ball->SinkBall();
}


bool Application2D::CheckVelocities(float _deltaTime)
{
	int movingBalls = 0;
	bool ballsMoving = true;
	glm::vec2 currentTotalVelocity = glm::vec2(0);

	for(int i = 0; i < m_allBalls.size(); i++)
	{
		if (m_allBalls[i]->GetVelocity().x > 0.1f || m_allBalls[i]->GetVelocity().y > 0.1f)
		{
			if(m_allBalls[i]->GetBallType() != WHITE)
				movingBalls++;
			
			if(movingBalls == 1 && m_firstBallHit == nullptr)
				m_firstBallHit = m_allBalls[i];

			currentTotalVelocity += m_allBalls[i]->GetVelocity();
		}
	}

	//glm::vec2 averageVelocity = glm::vec2(currentTotalVelocity.x / m_allBalls.size(), currentTotalVelocity.y / m_allBalls.size());

	if (currentTotalVelocity == glm::vec2(0) && currentTotalVelocity == m_previousTurnVelocity)
		ballsMoving = false;

	m_previousTurnVelocity = currentTotalVelocity;
	/*if(m_firstBallHit != nullptr)
		std::cout << m_firstBallHit->GetBallTexture()->getFilename() << std::endl;*/


	return ballsMoving;
}

void Application2D::HandleGameplay(aie::Input* _input, float _deltaTime)
{
	if(CheckVelocities(_deltaTime))//m_turnTimer < 7
	{
		//m_turnTimer += _deltaTime;
		m_turnProcessed = false;
		m_isShooting = false;
		AssessPlayerFirstBallHit();
		return;
	}
	

	if (!CheckVelocities(_deltaTime) && !m_turnProcessed)
	{
		m_velocityTimer += _deltaTime;
		if (m_velocityTimer > 5)
		{
			ProcessTurn();
			m_hasClicked = false;
			m_velocityTimer = 0;
			
			// Clear The first ball hit variable
			m_firstBallHit = nullptr;
		}		
	}

	// INPUT
	if (_input->isMouseButtonDown(0) && !m_isShooting && !m_hasClicked && !m_whiteBall->HasSunk())
	{
		m_firstBallHit = nullptr;
		if(m_whiteBallReset)
		{
			if(CalculateForce(m_mousePos, m_whiteBall->GetPosition()).x <= 0)
			{
				m_whiteBall->ApplyForce(CalculateForce(m_mousePos, m_whiteBall->GetPosition()), glm::vec2(0));
				m_hasClicked = true;
				m_isShooting = true;
				m_whiteBallReset = false;
			}
		}
		else
		{
			m_whiteBall->ApplyForce(CalculateForce(m_mousePos, m_whiteBall->GetPosition()), glm::vec2(0));
			m_hasClicked = true;
			m_isShooting = true;
		}
	}

	if (_input->isMouseButtonDown(1) && m_whiteBall->HasSunk())
	{
		m_firstBallHit = nullptr;
		PlaceWhiteBall();
	}

	if(_input->isKeyDown(aie::INPUT_KEY_Z))
	{
		m_firstBallHit = nullptr;
		m_whiteBall->SetPosition(m_mousePos);
	}
}

glm::vec2 Application2D::ScreenToWorld(glm::vec2 _screenPos)
{
    glm::vec2 worldPos = _screenPos;

    // move the centre of the screen to (0,0)
    worldPos.x -= (float)getWindowWidth() / 2;
    worldPos.y -= (float)getWindowHeight() / 2;

    // scale according to our extents
    worldPos.x *= 2.0f * m_extents / getWindowWidth();
    worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

    return worldPos;
}

glm::vec2 Application2D::WorldToScreen(glm::vec2 _worldPos)
{
	glm::vec2 screenPos = _worldPos;
	screenPos.x *= getWindowWidth() / (2.0f * m_extents);
	screenPos.y *= (m_aspectRatio * getWindowHeight()) / (2.0f * m_extents);
	
	screenPos.x += (float)getWindowWidth() / 2;
	screenPos.y += (float)getWindowHeight() / 2;

	return screenPos;
}

float Application2D::DegreeToRadian(float _degree)
 {
	return _degree * (float)(PI / (double)180.f);
 }

float Application2D::RadianToDegree(float _radian)
{
	return _radian * (float)(180.f / (double)PI);
}

float Application2D::GetAngleBetweenPoints(glm::vec2 _point1, glm::vec2 _point2)
{
	return atan2(_point2.y - _point1.y, _point2.x - _point1.x);
}

float Application2D::GetLengthBetweenPoints(glm::vec2 _point1, glm::vec2 _point2)
{
	return sqrt(pow(_point2.x - _point1.x, 2) + pow(_point2.y - _point1.y, 2) * 1.0);
}

glm::vec2 Application2D::CalculateForce(glm::vec2 _point1, glm::vec2 _point2)
{
	float angle = GetAngleBetweenPoints(_point1, _point2);
	float length = GetLengthBetweenPoints(_point1, _point2);

	if(length > 100)
	{
		length = 100 * 15;
	}
	else if (length < 10)
	{
		length = 10 * 5;
	}
	else
	{
		length *= 10;
	}
	
	glm::vec2 force = glm::vec2(cos(angle) * length, sin(angle) * length);

	//std::cout << std::to_string(force.x) << " - " << std::to_string(force.y) << std::endl;
	return force;
}