#pragma once
#include <string>

#include "PhysicsScene.h"

class SoftBody
{
public:
    static void BuildCircles(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings);
    static void BuildCirclesWithoutSprings(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings);
    static void BuildBoxes(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings);
    static void BuildBoxesWithoutSprings(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings);
};
