#pragma once
#include <glm/glm.hpp>

enum class MoveDir
{
	NO_MOVE = 0,
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct EnemyComponent
{
	int maxMoveDistance{ 0 };
	MoveDir moveDir{ MoveDir::NO_MOVE };
	glm::vec2 minDistance{ 0.f }, maxDistance{ 0.f }, startPos{ 0.f };
	bool distanceCalculated{ false }, maxMoveReached{ false };
};