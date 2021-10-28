#include "AIComponent.h"

AIComponent::AIComponent(glm::vec2 enemyPos, EnemyType enemyType)
{
	esm = nullptr;
	this->enemyPos = enemyPos;
	this->enemyType = enemyType;
}

const bool AIComponent::StateMachineCreated() 
{
	return esm;
}

void AIComponent::CreateStateMachine()
{
	esm = new EnemyStateMachine;
}
