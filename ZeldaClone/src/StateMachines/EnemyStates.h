#pragma once
#include "TState.h"
#include "EnemyStateMachine.h"
class Timer;
class Entity;

class EnemyIdleState : public TState<class EnemyStateMachine>
{
public:
	virtual ~EnemyIdleState() {};
	void OnEnter(EnemyStateMachine* pOwner, Entity& entity) override;
	void OnExit(EnemyStateMachine* pOwner, Entity& entity) override;
	void Execute(EnemyStateMachine* pOwner, Entity& entity) override;
};

class EnemyAttackState : public TState<class EnemyStateMachine>
{
public:
	virtual ~EnemyAttackState() {};
	void OnEnter(EnemyStateMachine* pOwner, Entity& entity) override;
	void OnExit(EnemyStateMachine* pOwner, Entity& entity) override;
	void Execute(EnemyStateMachine* pOwner, Entity& entity) override;
};

class PatrolState : public TState<class EnemyStateMachine>
{
public:
	virtual ~PatrolState() {};
	void OnEnter(EnemyStateMachine* pOwner, Entity& entity) override;
	void OnExit(EnemyStateMachine* pOwner, Entity& entity) override;
	void Execute(EnemyStateMachine* pOwner, Entity& entity) override;
};

class HurtState : public TState<class EnemyStateMachine>
{
private:
	bool itemCollected = false;
	Entity* trigItem;

public:
	virtual ~HurtState() {};
	void OnEnter(EnemyStateMachine* pOwner, Entity& entity) override;
	void OnExit(EnemyStateMachine* pOwner, Entity& entity) override;
	void Execute(EnemyStateMachine* pOwner, Entity& entity) override;
};