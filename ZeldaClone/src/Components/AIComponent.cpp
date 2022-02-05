#include "AIComponent.h"
#include "../StateMachines/NewStateMachine.h"
#include "../StateMachines/NewEnemyStates.h"
#include "../Logger/Logger.h"

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
	esm = new StateMachine();
	esm->AddState(std::make_unique<EnemyIdleState>());
	created = true;
}
