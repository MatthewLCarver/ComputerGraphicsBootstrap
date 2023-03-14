#include "PhysicsApp.h"

#include <iostream>
#include <glm/ext.hpp>

#include "Box.h"
#include "Circle.h"
#include "Texture.h"
#include "Font.h"
#include "Gizmos.h"
#include "Input.h"
#include "PhysicsScene.h"
#include "Demos.h"
#include "Plane.h"
#include "SoftBody.h"
#include "Spring.h"

PhysicsApp::PhysicsApp()
{
	
}

PhysicsApp::~PhysicsApp()
{
}

bool PhysicsApp::Startup()
{
    //Increase the 2D line count to maximise the objects we can draw
    aie::Gizmos::create(225U, 225U, 65535U, 65535U);

    m_2dRenderer = new aie::Renderer2D();
	
    // TODO: remember to change this when redistributing a build!
    // the following path would be used instead: "./font/consolas.ttf"
    m_font = new aie::Font("../bin/font/consolas.ttf", 32);

    m_physicsScene = new PhysicsScene();

    m_physicsScene->SetTimeStep(0.01f);

    DemoStartUp(1);

    return true;
}

void PhysicsApp::Shutdown()
{
    delete m_font;
    delete m_2dRenderer;
}

void PhysicsApp::Update(float _deltaTime)
{
    // input example
    aie::Input* input = aie::Input::getInstance();

    aie::Gizmos::clear();

    m_physicsScene->Update(_deltaTime);
	m_physicsScene->Draw();

	DemoUpdates(input, _deltaTime);
	
    // exit the application
    if(input->isKeyDown(aie::INPUT_KEY_ESCAPE))
        quit();
}

void PhysicsApp::draw()
{

	// wipe the screen to the background colour
	clearScreen();

	

	// begin drawing sprites
	m_2dRenderer->begin();
	
    // draw your stuff here!
    aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
                                          -m_extents / m_aspectRatio,
                                          m_extents / m_aspectRatio,
                                          -1.0f, 1.0f));

	
	// 2d tiled array background
	/*for (int y = 0; y < getWindowHeight() / m_backgroundTexture->getHeight() + 2; y++)
	{
		for (int x = 0; x < getWindowWidth() / m_backgroundTexture->getWidth(); x++)
		{
			m_2dRenderer->drawSprite(m_backgroundTexture,
				x * m_backgroundTexture->getWidth() + m_backgroundTexture->getWidth() / 2,
				y * m_backgroundTexture->getHeight() - m_backgroundTexture->getHeight() / 2,
				m_backgroundTexture->getWidth(),
				m_backgroundTexture->getHeight(),
				0, 0);
		}
	}*/

	/*m_2dRenderer->drawSprite(m_tableTexture,20, -6,
		getWindowWidth()-25, getWindowHeight() - 90, 0, 0, 00, 0);*/
	
    // output some text, uses the last used colour
    //m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);
	
    // done drawing sprites
    m_2dRenderer->end();
}



void PhysicsApp::DemoStartUp(int _num)
{
#ifdef NewtonsFirstLaw
	m_physicsScene->SetGravity(glm::vec2(0, -69));
	Circle* ball;
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif
#ifdef NewtonsSecondLaw
	 m_physicsScene->SetGravity(glm::vec2(0, -69));
	Circle* ball;
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif
#ifdef NewtonsThirdLaw
	m_physicsScene->SetGravity(glm::vec2(0, 0));  // turn off gravity

	Circle* ball1 = new Circle(glm::vec2(-10, 0), glm::vec2(3, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(-3, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	//ball1->ApplyForceToActor(ball2, glm::vec2(-2, 0));
#endif
#ifdef SimulatingACollision
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForce(glm::vec2(30, 0));
	ball2->ApplyForce(glm::vec2(-15, 0));

#endif
#ifdef SimulatingRocket
	m_physicsScene->SetGravity(glm::vec2(0, 0));//-GRAVITY));

	static Circle* ball1 = new Circle(glm::vec2(0, 0), glm::vec2(0), 800.0f, 4, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball1);
#endif
#ifdef CircleToPlane
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Plane* plane = new Plane(glm::vec2(0, 1), -30);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(plane);
#endif
#ifdef CircleToCircleNewtonsCradle
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 3.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(40, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 0, 1, 1));
	Circle* ball4 = new Circle(glm::vec2(70, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);

	ball1->ApplyForce(glm::vec2(100, 0));
	ball2->ApplyForce(glm::vec2(0, 0));
	ball3->ApplyForce(glm::vec2(0, 0));
	ball4->ApplyForce(glm::vec2(0, 0));
#endif
#ifdef SymetricalNewtonsCradle
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(-70, 2), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1), false);
	Circle* ball2 = new Circle(glm::vec2(-40, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1), false);
	Circle* ball3 = new Circle(glm::vec2(-10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 0, 1, 1), false);
	Circle* ball4 = new Circle(glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 1, 0, 1), false);
	Circle* ball5 = new Circle(glm::vec2(40, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 1, 1), false);
	Circle* ball6 = new Circle(glm::vec2(70, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 1, 1), false);
	Circle* ball7 = new Circle(glm::vec2(-75, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 1, 1, 1), false);
	Plane* plane1 = new Plane(glm::vec2(1, 0), -95);
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -95);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);
	m_physicsScene->AddActor(ball6);
	m_physicsScene->AddActor(ball7);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);

	ball1->ApplyForce(glm::vec2(100, 0), glm::vec2(0, 0));
#endif
#ifdef AsymmetricalNewtonsCradle
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(-70, 2), glm::vec2(0), 8.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-40, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(-10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 0, 1, 1));
	Circle* ball4 = new Circle(glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 1, 0, 1));
	Circle* ball5 = new Circle(glm::vec2(40, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 1, 1));
	Circle* ball6 = new Circle(glm::vec2(70, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 1, 1));
	Circle* ball7 = new Circle(glm::vec2(-75, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 1, 1, 1));
	Plane* plane1 = new Plane(glm::vec2(1, 0), -95);
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -95);
	Plane* plane3 = new Plane(glm::vec2(0, 1), -50);
	Plane* plane4 = new Plane(glm::vec2(0, -1), -50);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);
	m_physicsScene->AddActor(ball6);
	m_physicsScene->AddActor(ball7);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	m_physicsScene->AddActor(plane4);

	ball1->ApplyForce(glm::vec2(2000, 0), glm::vec2(0, 2));
#endif
#ifdef BoxToPlaneCollision
	m_physicsScene->SetGravity(glm::vec2(0, -GRAVITY));

	Box* box1 = new Box(glm::vec2 (0, 0), glm::vec2(0), 10.0f, glm::vec2(5), glm::vec4(1, 0, 0, 1));
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50);

	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(plane1);
#endif
#ifdef BoxtoCircleCollision
	m_physicsScene->SetGravity(glm::vec2(0, -GRAVITY));

	// create a box
	Box* box1 = new Box(glm::vec2 (0, 0), glm::vec2(0), 10.0f, glm::vec2(5), glm::vec4(1, 0, 0, 1), false);
	Circle* circle1 = new Circle(glm::vec2(8, 20), glm::vec2(0), 4.0f, 4, glm::vec4(0, 0, 1, 1), false);
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50);

	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(circle1);
	m_physicsScene->AddActor(plane1);
#endif
#ifdef BoxtoBoxCollision
	m_physicsScene->SetGravity(glm::vec2(0, -GRAVITY));

	Box* box1 = new Box(glm::vec2 (0, 0), glm::vec2(0), 20.0f, glm::vec2(5), glm::vec4(1, 1, 0, 1), false, 0.0f, .1f);
	Box* box2 = new Box(glm::vec2 (8.0f, 20), glm::vec2(0), 20.0f, glm::vec2(5), glm::vec4(0, 1, 1, 1), false, 0.0f, .1f);
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50);

	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);
	m_physicsScene->AddActor(plane1);
#endif
#ifdef PhysicsBucket
	m_physicsScene->SetGravity(glm::vec2(0, -GRAVITY));

	// create a box
	Box* box1 = new Box(glm::vec2 (0, 0), glm::vec2(0), 10.0f, glm::vec2(10, 2), glm::vec4(1, 0, 0, 1), true, 0, .3f);
	Box* box2 = new Box(glm::vec2 (10, 0), glm::vec2(0), 10.0f, glm::vec2(2, 10), glm::vec4(1, 0, 0, 1), true, 0, .3f);
	Box* box3 = new Box(glm::vec2 (-10, 0), glm::vec2(0), 10.0f, glm::vec2(2, 10), glm::vec4(1, 0, 0, 1), true, 0, .3f);
	Box* box4 = new Box(glm::vec2 (-30, 0), glm::vec2(0), 10.0f, glm::vec2(10), glm::vec4(1, 0, 0, 1), true, 0, .3f);
	Box* box5 = new Box(glm::vec2 (-30, 0), glm::vec2(0), 10.0f, glm::vec2(10, 2), glm::vec4(1, 0, 0, 1), true, 0, .3f);
	Circle* circle1 = new Circle(glm::vec2(8, 20), glm::vec2(0), 1.0f, 4, glm::vec4(0, 0, 1, 1), false);
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50);
	box1->SetKinematic(true);
	box2->SetKinematic(true);
	box3->SetKinematic(true);

	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);
	m_physicsScene->AddActor(box3);
	m_physicsScene->AddActor(box4);
	m_physicsScene->AddActor(box5);
	m_physicsScene->AddActor(circle1);
	m_physicsScene->AddActor(plane1);
#endif
#ifdef SpringExperiment
	m_physicsScene->SetGravity(glm::vec2(0, -GRAVITY));

	// create a box
	Circle* circle1 = new Circle(glm::vec2(0, 20), glm::vec2(0), 10.0f, 4, glm::vec4(0, 0, 1, 1), true);
	Circle* circle2 = new Circle(glm::vec2(5, 10), glm::vec2(0), 10.0f, 4, glm::vec4(0, 0, 1, 1), false);
	Spring* spring1 = new Spring(circle1, circle2, 40, 0.001f, 0);
	Circle* circle3 = new Circle(glm::vec2(20, 10), glm::vec2(0), 10.0f, 4, glm::vec4(0, 0, 1, 1), false);
	Spring* spring2 = new Spring(circle2, circle3, 40, 0.001f, 0);
	Box* box1 = new Box(glm::vec2 (30, 30), glm::vec2(0), 10.0f, glm::vec2(5), glm::vec4(1, 0, 0, 1), false, 0.5f, 0.75f);
	Spring* spring3 = new Spring(circle3, box1, 40, 0.001f, 0);
	
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50);
	
	m_physicsScene->AddActor(circle1);
	m_physicsScene->AddActor(circle2);
	m_physicsScene->AddActor(spring1);
	m_physicsScene->AddActor(circle3);
	m_physicsScene->AddActor(spring2);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(spring3);
	
	m_physicsScene->AddActor(plane1);
#endif
#ifdef SoftbodyTest
	m_physicsScene->SetGravity(glm::vec2(0, -GRAVITY));

	Plane* plane = new Plane(glm::vec2(0, 1), -50);
	m_physicsScene->AddActor(plane);

	/*Plane* planeD = new Plane(glm::vec2(.707f, .707f), -50);
	m_physicsScene->AddActor(planeD);*/

	std::vector<std::string> sb;
	sb.push_back("000000---00000");
	sb.push_back("000000---00000");
	sb.push_back("000000---00000");
	sb.push_back("00000000000000");
	sb.push_back("00000000000000");
	/*sb.push_back("00000000000000");
	sb.push_back("00000000000000");
	sb.push_back("00000000000000");
	sb.push_back("----0000000---");
	sb.push_back("----0000000---");
	sb.push_back("----0000000---");
	sb.push_back("----0000000---");
	sb.push_back("--00000000000-");
	sb.push_back("---000---000--");
	sb.push_back("----00---00---");
	sb.push_back("-----00000----");
	sb.push_back("------000-----");
	sb.push_back("-------0------");*/
	

	std::vector<std::string> sc;
	sc.push_back("..00..");
	sc.push_back("..00..");
	sc.push_back("000000");
	sc.push_back("000000");
	sc.push_back("..00..");
	sc.push_back("..00..");

	std::vector<std::string> sd;
	sd.push_back("..00..");
	sd.push_back("..00..");
	sd.push_back("000000");
	sd.push_back("000000");
	sd.push_back("..00..");
	sd.push_back("..00..");


	SoftBody::BuildBoxes(m_physicsScene, glm::vec2(-50, -49), -10.0f, 5000.0f, 5.1f, sb);
	//SoftBody::BuildBoxesWithoutSprings(m_physicsScene, glm::vec2(-50, -49), 500.0f, 5.0f, 5.f, sb);
	//SoftBody::BuildCircles(m_physicsScene, glm::vec2(0, -30), 5.0f, 5.0f, 6.f, sc);
	//SoftBody::BuildBoxes(m_physicsScene, glm::vec2(50, -25), 500.0f, 5.0f, 6.f, sd);

	/*Circle* ball = new Circle(glm::vec2(0, -30), glm::vec2(0), 2.f, 2, glm::vec4(0, 1, 0, 1), false);
	m_physicsScene->AddActor(ball);*/
#endif
#ifdef MouseTest
	m_physicsScene->SetGravity(glm::vec2(0, -GRAVITY));

	Plane* plane = new Plane(glm::vec2(0, 1), -50);
	m_physicsScene->AddActor(plane);

	Circle* ball = new Circle(glm::vec2(0, -50), glm::vec2(0), 2.f, 2, glm::vec4(0, 1, 0, 1), false);
	m_physicsScene->AddActor(ball);
	
	Box* box = new Box(glm::vec2(10, -50), glm::vec2(0), 2.f, glm::vec2(2, 2), glm::vec4(1, 0, 0, 1),
		false, 0.0f, 0.05f);
	m_physicsScene->AddActor(box);
#endif
	
	//PoolSetup();
}

void PhysicsApp::DemoUpdates(aie::Input* _input, float _dt)
{
#ifdef SimulatingRocket
    Circle* rocketCircle = dynamic_cast<Circle*>(m_physicsScene->GetActors()[0]);
    float M =20.f;
    if(rocketCircle != nullptr && rocketCircle->GetMass() > 0.0f)
    {
        rocketCircle->SetMass(rocketCircle->GetMass() - M);
        if(rocketCircle->GetMass() <= 0.0f)
            return;
		
        Circle* exhaustCircle = new Circle(rocketCircle->GetPosition() + glm::vec2(0, -rocketCircle->GetRadius() - .5f),
            glm::vec2(0), M, M/20,
            glm::vec4(0, 1, 0, 1));
        m_physicsScene->AddActor(exhaustCircle);
        CullPhysicsActors();
		
        rocketCircle->ApplyForceToActor(exhaustCircle, glm::vec2(0, -(M*20)));
		
    }
#endif
#ifdef BoxtoBoxCollision
    // DEBUGGING CODE
    /*Box* box1 = dynamic_cast<Box*>(m_physicsScene->GetActors()[0]);
    Box* box2 = dynamic_cast<Box*>(m_physicsScene->GetActors()[1]);
    std::cout << "Box1 Velocity: " << box1->GetVelocity().x << ", " << box1->GetVelocity().y << "			Box2 Velocity: " << box2->GetVelocity().x << ", " << box2->GetVelocity().y << std::endl;
    std::cout << "Box1 Rotation: " << box1->GetOrientation() << "				Box2 Rotation: " << box2->GetOrientation() << std::endl;
    std::cout << "Box1 Angular Velocity: " << box1->GetAngularVelocity() << "			Box2 Angular Velocity: " << box2->GetAngularVelocity() << std::endl;*/	
    #endif
#ifdef SoftbodyTest
    // DEBUGGING CODE
    Box* box1 = dynamic_cast<Box*>(m_physicsScene->GetActors()[1]);
    std::cout << "Box1 Angular Velocity: " << box1->GetAngularVelocity() << std::endl;
#endif


	/*if (_input->isMouseButtonDown(0))
    {
        int xScreen, yScreen;
        _input->getMouseXY(&xScreen, &yScreen);
        glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));

        aie::Gizmos::add2DCircle(worldPos, .5f, 24, glm::vec4(0, 0, 1, 1));
    }*/

	// Moves the camera in Arrows
	/*float camPosX;
	float camPosY;
	m_2dRenderer->getCameraPos(camPosX, camPosY);

	if (_input->isKeyDown(aie::INPUT_KEY_UP))
		camPosY += 500.0f * _dt;

	if (_input->isKeyDown(aie::INPUT_KEY_DOWN))
		camPosY -= 500.0f * _dt;

	if (_input->isKeyDown(aie::INPUT_KEY_LEFT))
		camPosX -= 500.0f * _dt;

	if (_input->isKeyDown(aie::INPUT_KEY_RIGHT))
		camPosX += 500.0f * _dt;

	m_2dRenderer->setCameraPos(camPosX, camPosY);*/
	
}

float PhysicsApp::GetAngleBetweenPoints(glm::vec2 _point1, glm::vec2 _point2)
{
	float angle = atan2(_point2.y - _point1.y, _point2.x - _point1.x);
	return angle;
}

float PhysicsApp::RadianToDegree(float _radian)
{
	return _radian * (180.f / PI);
}

float PhysicsApp::DegreeToRadian(float _degree)
{
    return _degree * (PI / 180.f);
}


glm::vec2 PhysicsApp::ScreenToWorld(glm::vec2 _screenPos)
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