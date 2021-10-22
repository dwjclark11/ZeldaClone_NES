#pragma once
#include "../StateMachines/EnemyStateMachine.h"
#include "../StateMachines/PlayerStateMachine.h"
#include "../Utilities/Timer.h"
#include <glm/glm.hpp>
#include <memory>

class AIComponent
{
	EnemyStateMachine* esm;
	glm::vec2 enemyPos;
public: 
	Timer aiTimer;
	Timer deathTimer;

	AIComponent(glm::vec2 enemyPos = glm::vec2(0, 0));

	EnemyStateMachine& GetEnemyStateMachine() { return *esm; }
	void GarbageCollect() { delete esm; }
	glm::vec2& GetEnemyPos() { return enemyPos; }
};