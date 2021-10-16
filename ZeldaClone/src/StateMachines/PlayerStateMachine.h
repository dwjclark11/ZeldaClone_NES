#pragma once
#include "TStateMachine.h"
#include "PlayerStates.h"

class PlayerStateMachine : public TStateMachine<PlayerStateMachine>
{
private:
	TState<PlayerStateMachine>* idleState;
	TState<PlayerStateMachine>* moveState;
	TState<PlayerStateMachine>* attackState;
	TState<PlayerStateMachine>* collectItemState;
	TState<PlayerStateMachine>* hurtState;
	bool isRunning = true;

public:
	PlayerStateMachine();
	virtual ~PlayerStateMachine();
	void Update(Entity& entity);

	inline void Stop(bool running) { isRunning = running; }
	inline bool IsRunning() { return isRunning; }

	friend class IdleState;
	friend class MoveState;
	friend class AttackState;
	friend class CollectItemState;
	friend class PlayerHurtState;
};

