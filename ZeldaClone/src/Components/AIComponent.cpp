#include "AIComponent.h"
#include "../StateMachines/NewStateMachine.h"
#include "../StateMachines/NewEnemyStates.h"
#include "../StateMachines/BossStates.h"
#include "../Logger/Logger.h"

AIComponent::AIComponent(glm::vec2 enemyPos, EnemyType enemyType, EnemyBossType bossType, bool boss)
{
	esm = nullptr;
	this->enemyPos = enemyPos;
	this->enemyType = enemyType;
	this->bossType = bossType;
	this->boss = boss;
	
}


const bool AIComponent::StateMachineCreated() 
{
	return esm;
}

void AIComponent::CreateStateMachine()
{
	// This needs to be changed to a unique ptr!
	esm = new StateMachine();
	
	if (!boss)
		esm->AddState(std::make_unique<EnemyIdleState>());
	else
		esm->AddState(std::make_unique<BossIdleState>());
	
	created = true;
}
