#pragma once
#include "TState.h"
#include "PlayerStateMachine.h"
class Timer;
class Entity;

class IdleState : public TState<class PlayerStateMachine>
{
public:
	void OnEnter(PlayerStateMachine* pOwner, Entity& entity) override;
	void OnExit(PlayerStateMachine* pOwner, Entity& entity) override;
	void Execute(PlayerStateMachine* pOwner, Entity& entity) override;
};

class AttackState : public TState<class PlayerStateMachine>
{
public:
	void OnEnter(PlayerStateMachine* pOwner, Entity& entity) override;
	void OnExit(PlayerStateMachine* pOwner, Entity& entity) override;
	void Execute(PlayerStateMachine* pOwner, Entity& entity) override;
};

class MoveState : public TState<class PlayerStateMachine>
{
public:
	void OnEnter(PlayerStateMachine* pOwner, Entity& entity) override;
	void OnExit(PlayerStateMachine* pOwner, Entity& entity) override;
	void Execute(PlayerStateMachine* pOwner, Entity& entity) override;
};

class CollectItemState : public TState<class PlayerStateMachine>
{
private:
	//bool itemCollected = false;
	//bool movedTrigItem = false;

public:
	void OnEnter(PlayerStateMachine* pOwner, Entity& entity) override;
	void OnExit(PlayerStateMachine* pOwner, Entity& entity) override;
	void Execute(PlayerStateMachine* pOwner, Entity& entity) override;
};

class PlayerHurtState : public TState<class PlayerStateMachine>
{
private:

public:
	void OnEnter(PlayerStateMachine* pOwner, Entity& entity) override;
	void OnExit(PlayerStateMachine* pOwner, Entity& entity) override;
	void Execute(PlayerStateMachine* pOwner, Entity& entity) override;
};


class PlayerDeathState: public TState<class PlayerStateMachine>
{
private:

public:
	void OnEnter(PlayerStateMachine* pOwner, Entity& entity) override;
	void OnExit(PlayerStateMachine* pOwner, Entity& entity) override;
	void Execute(PlayerStateMachine* pOwner, Entity& entity) override;
};

class PlayerStairsState : public TState<class PlayerStateMachine>
{
private:
	int steps = 0;
public:
	void OnEnter(PlayerStateMachine* pOwner, Entity& entity) override;
	void OnExit(PlayerStateMachine* pOwner, Entity& entity) override;
	void Execute(PlayerStateMachine* pOwner, Entity& entity) override;
};