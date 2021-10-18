#include "PlayerStateMachine.h"


PlayerStateMachine::PlayerStateMachine()
{
	Logger::Log("Player State Machine Created");
	idleState 			= new IdleState();
	moveState 			= new MoveState();
	attackState 		= new AttackState();
	collectItemState 	= new CollectItemState();
	hurtState 			= new PlayerHurtState();
	deathState 			= new PlayerDeathState();
	
	TStateMachine<PlayerStateMachine>::Init(this, idleState);
}

PlayerStateMachine::~PlayerStateMachine()
{
	// Delete all of the active states
	delete idleState;
	delete moveState; 
	delete attackState;
	delete collectItemState;
	delete hurtState;
	delete deathState;
}

void PlayerStateMachine::Update(Entity& entity)
{
	TStateMachine<PlayerStateMachine>::Update(entity);
}
