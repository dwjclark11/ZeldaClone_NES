// EnemyStateMachine.h
#pragma once
#include "TStateMachine.h"
#include "EnemyStates.h"

class EnemyStateMachine : public TStateMachine<EnemyStateMachine>
{
private:
	TState<EnemyStateMachine>* idleState;
	TState<EnemyStateMachine>* patrolState;
	TState<EnemyStateMachine>* attackState;
	TState<EnemyStateMachine>* hurtState;
	TState<EnemyStateMachine>* deathState;
	bool isRunning = true;
	

public:
	EnemyStateMachine();
	virtual ~EnemyStateMachine();
	void Update(Entity& entity);
	inline void Stop(bool running) { isRunning = running; }
	inline bool IsRunning() { return isRunning; }

	friend class EnemyIdleState;
	friend class PatrolState;
	friend class EnemyAttackState;
	friend class HurtState;
	friend class EnemyDeathState;
};