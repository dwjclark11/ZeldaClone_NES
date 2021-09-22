#pragma once
#include "TState.h"
#include "PlayerStateMachine.h"
class Timer;
class Entity;

class IdleState : public TState<class PlayerStateMachine>
{
public:
	void OnEnter(PlayerStateMachine* pOwner) override;
	void OnExit(PlayerStateMachine* pOwner) override;
	void Execute(PlayerStateMachine* pOwner) override;
};

class AttackState : public TState<class PlayerStateMachine>
{
public:
	void OnEnter(PlayerStateMachine* pOwner) override;
	void OnExit(PlayerStateMachine* pOwner) override;
	void Execute(PlayerStateMachine* pOwner) override;
};

class MoveState : public TState<class PlayerStateMachine>
{
public:
	void OnEnter(PlayerStateMachine* pOwner) override;
	void OnExit(PlayerStateMachine* pOwner) override;
	void Execute(PlayerStateMachine* pOwner) override;
};

class CollectItemState : public TState<class PlayerStateMachine>
{
private:
	bool itemCollected = false;
	Entity* trigItem;

public:
	void OnEnter(PlayerStateMachine* pOwner) override;
	void OnExit(PlayerStateMachine* pOwner) override;
	void Execute(PlayerStateMachine* pOwner) override;
};
