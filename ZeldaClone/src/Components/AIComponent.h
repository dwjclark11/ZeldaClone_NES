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

	AIComponent(glm::vec2 enemyPos = glm::vec2(0, 0));
	//virtual ~AIComponent()
	//{
	//	delete esm;
	//}

	EnemyStateMachine* GetEnemyStateMachine() { return esm; }
	void GarbageCollect() { delete esm; }
	glm::vec2& GetEnemyPos() { return enemyPos; }
	//std::unique_ptr<EnemyStateMachine>& GetEnemyStateMachine() { return esm; }
};