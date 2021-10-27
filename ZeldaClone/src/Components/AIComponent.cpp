#include "AIComponent.h"

AIComponent::AIComponent(glm::vec2 enemyPos)
{
	//esm = new EnemyStateMachine;
	esm = nullptr;
	this->enemyPos = enemyPos;
}

bool AIComponent::StateMachineCreated()
{
	return esm;
}

void AIComponent::CreateStateMachine()
{
	esm = new EnemyStateMachine;
}
