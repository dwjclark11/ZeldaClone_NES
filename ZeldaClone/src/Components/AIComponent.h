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
	bool stunned;

public: 
	Timer aiTimer;
	Timer deathTimer;
	Timer stunTimer;

	AIComponent(glm::vec2 enemyPos = glm::vec2(0, 0));

	EnemyStateMachine& GetEnemyStateMachine() { return *esm; }
	void GarbageCollect() { delete esm; }
	glm::vec2& GetEnemyPos() { return enemyPos; }
	void SetStunned(bool stun) { stunned = stun; }
	bool GetStunned() { return stunned; }
};