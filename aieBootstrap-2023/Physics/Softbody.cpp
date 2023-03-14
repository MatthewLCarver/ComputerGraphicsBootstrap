#include "Circle.h"
#include "Box.h"
#include "SoftBody.h"
#include "Spring.h"

void SoftBody::BuildCircles(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings)
{
    int numColumns = _strings.size();
    int numRows = _strings[0].length();

    // traverse across the array and add balls where the ascii art says they should be
    Circle** circles = new Circle*[numRows* numColumns];
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            if (_strings[j][i] == '0')
            {
                circles[i*numColumns + j] = new Circle(_position + glm::vec2(i, j)*_spacing, glm::vec2(0, 0),
                    1.0f, 2.0f, glm::vec4(1, 0, 0, 1), false, false);
                _scene->AddActor(circles[i*numColumns + j]);
            }
            else
            {
                circles[i*numColumns + j] = nullptr;
            }
        }
    }

    // Add Spring Connectors
    for (int i = 1; i < numRows; i++)
    {
        for (int j = 1; j < numColumns; j++)
        {
            Circle* s11 = circles[i*numColumns + j];
            Circle* s01 = circles[(i-1)*numColumns + j];
            Circle* s10 = circles[i*numColumns + j-1];
            Circle* s00 = circles[(i-1)*numColumns + j-1];
		
            // make springs to cardinal neighbours
            if (s11 && s01)
                _scene->AddActor(new Spring(s11, s01, _damping, _springForce, _spacing));
            if (s11 && s10)
                _scene->AddActor(new Spring(s11, s10, _damping, _springForce, _spacing));
            if (s10 && s00)
                _scene->AddActor(new Spring(s10, s00, _damping, _springForce, _spacing));
            if (s01 && s00)
                _scene->AddActor(new Spring(s01, s00, _damping, _springForce, _spacing));

            bool endOfJ = j == numColumns - 1;
            bool endOfI = i == numRows - 1;

            Circle* s22 = (!endOfI && !endOfJ) ? circles[(i+1)*numColumns + (j+1)] : nullptr;
            Circle* s02 = !endOfJ ? circles[(i - 1)*numColumns + (j+1)] : nullptr;
            Circle* s20 = !endOfI ? circles[(i+1)*numColumns + j - 1] : nullptr;

            //Then add springs between s00 and s02, s22 and s20, and so on, to have potentially another four springs
            if(s00 && s02)
                _scene->AddActor(new Spring(s00, s02, _damping, _springForce, _spacing * 2.f));
            if(s22 && s20)
                _scene->AddActor(new Spring(s22, s20, _damping, _springForce, _spacing * 2.f));
            if(s22 && s02)
                _scene->AddActor(new Spring(s22, s02, _damping, _springForce, _spacing * 2.f));
            if(s20 && s00)
                _scene->AddActor(new Spring(s20, s00, _damping, _springForce, _spacing * 2.f));
            if(s11 && s02)
                _scene->AddActor(new Spring(s11, s02, _damping, _springForce, _spacing * 1.414f));
            if(s11 && s20)
                _scene->AddActor(new Spring(s11, s20, _damping, _springForce, _spacing * 1.414f));
            if(s11 && s22)
                _scene->AddActor(new Spring(s11, s22, _damping, _springForce, _spacing * 1.414f));
            
        }
    }
}

void SoftBody::BuildCirclesWithoutSprings(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings)
{
    int numColumns = _strings.size();
    int numRows = _strings[0].length();

    // traverse across the array and add balls where the ascii art says they should be
    Circle** circles = new Circle*[numRows* numColumns];
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            if (_strings[j][i] == '0')
            {
                circles[i*numColumns + j] = new Circle(_position + glm::vec2(i, j)*_spacing, glm::vec2(0, 0),
                    1.0f, 4.0f, glm::vec4(1, 0, 0, 1), false, false);
                _scene->AddActor(circles[i*numColumns + j]);
            }
            else
            {
                circles[i*numColumns + j] = nullptr;
            }
        }
    }
}

#include "Circle.h"
#include "Box.h"
#include "SoftBody.h"
#include "Spring.h"

void SoftBody::BuildBoxes(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings)
{
    int numColumns = _strings.size();
    int numRows = _strings[0].length();

    // traverse across the array and add balls where the ascii art says they should be
    Box** boxes = new Box*[numRows* numColumns];
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            if (_strings[j][i] == '0')
            {
                boxes[i * numColumns + j] =
                    new Box(glm::vec2(i, j) * _spacing + _position,
                    glm::vec2(0, 0),
                    0.0f,
                    5.0f,
                    glm::vec2(2.5),
                    glm::vec4(1, 0, 0, 1),
                    false)
                ;
                _scene->AddActor(boxes[i*numColumns + j]);
            }
            else
            {
                boxes[i*numColumns + j] = nullptr;
            }
        }
    }

    // Add Spring Connectors
    for (int i = 1; i < numRows; i++)
    {
        for (int j = 1; j < numColumns; j++)
        {
            Box* s11 = boxes[i*numColumns + j];
            Box* s01 = boxes[(i-1)*numColumns + j];
            Box* s10 = boxes[i*numColumns + j-1];
            Box* s00 = boxes[(i-1)*numColumns + j-1];
		
            // make springs to cardinal neighbours
            if (s11 && s01)
                _scene->AddActor(new Spring(s11, s01, _damping, _springForce, _spacing));
            if (s11 && s10)
                _scene->AddActor(new Spring(s11, s10, _damping, _springForce, _spacing));
            if (s10 && s00)
                _scene->AddActor(new Spring(s10, s00, _damping, _springForce, _spacing));
            if (s01 && s00)
                _scene->AddActor(new Spring(s01, s00, _damping, _springForce, _spacing));

            bool endOfJ = j == numColumns - 1;
            bool endOfI = i == numRows - 1;

            Box* s22 = (!endOfI && !endOfJ) ? boxes[(i+1)*numColumns + (j+1)] : nullptr;
            Box* s02 = !endOfJ ? boxes[(i - 1)*numColumns + (j+1)] : nullptr;
            Box* s20 = !endOfI ? boxes[(i+1)*numColumns + j - 1] : nullptr;

            //Then add springs between s00 and s02, s22 and s20, and so on, to have potentially another four springs
            if(s00 && s02)
                _scene->AddActor(new Spring(s00, s02, _damping, _springForce, _spacing * 2.f));
            if(s22 && s20)
                _scene->AddActor(new Spring(s22, s20, _damping, _springForce, _spacing * 2.f));
            if(s22 && s02)
                _scene->AddActor(new Spring(s22, s02, _damping, _springForce, _spacing * 2.f));
            if(s20 && s00)
                _scene->AddActor(new Spring(s20, s00, _damping, _springForce, _spacing * 2.f));
            if(s11 && s02)
                _scene->AddActor(new Spring(s11, s02, _damping, _springForce, _spacing * 1.414f));
            if(s11 && s20)
                _scene->AddActor(new Spring(s11, s20, _damping, _springForce, _spacing * 1.414f));
            if(s11 && s22)
                _scene->AddActor(new Spring(s11, s22, _damping, _springForce, _spacing * 1.414f));
            
        }
    }
}

void SoftBody::BuildBoxesWithoutSprings(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings)
{
    int numColumns = _strings.size();
    int numRows = _strings[0].length();

    // traverse across the array and add balls where the ascii art says they should be
    Box** boxes = new Box*[numRows* numColumns];
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            if (_strings[j][i] == '0')
            {
                boxes[i*numColumns + j] =
                    new Box(glm::vec2(i, j) * _spacing + _position,
                    glm::vec2(0, 0),
                    0.0f,
                    10.0f,
                    glm::vec2(2.5),
                    glm::vec4(1, 0, 0, 1),
                    false)
                ;
                _scene->AddActor(boxes[i*numColumns + j]);
            }
            else
            {
                boxes[i*numColumns + j] = nullptr;
            }
        }
    }
}

