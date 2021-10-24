// EnemyStateMachine.cpp
#include "EnemyStateMachine.h"


EnemyStateMachine::EnemyStateMachine()
{
	Logger::Log("Enemy State Machine Created");
	idleState = new EnemyIdleState;
	patrolState = new PatrolState;
	attackState = new EnemyAttackState;
	hurtState = new HurtState;
	deathState = new EnemyDeathState;
	stunState = new EnemyStunnedState;
	TStateMachine<EnemyStateMachine>::Init(this, idleState);
}

EnemyStateMachine::~EnemyStateMachine()
{
	// Delete all of the active states
	Logger::Log("Enemy State Machine Destroyed");
	delete idleState;
	delete patrolState; 
	delete attackState;
	delete hurtState;
	delete deathState;
	delete stunState;
}

void EnemyStateMachine::Update(Entity& entity)
{
	TStateMachine<EnemyStateMachine>::Update(entity);
}