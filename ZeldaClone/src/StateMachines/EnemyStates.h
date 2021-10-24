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
	// Add private variables as needed
	
public:
	virtual ~HurtState() {};
	void OnEnter(EnemyStateMachine* pOwner, Entity& entity) override;
	void OnExit(EnemyStateMachine* pOwner, Entity& entity) override;
	void Execute(EnemyStateMachine* pOwner, Entity& entity) override;
};

class EnemyDeathState : public TState<class EnemyStateMachine>
{
private:
	// Add private variables as needed

public:
	virtual ~EnemyDeathState() {};
	void OnEnter(EnemyStateMachine* pOwner, Entity& entity) override;
	void OnExit(EnemyStateMachine* pOwner, Entity& entity) override;
	void Execute(EnemyStateMachine* pOwner, Entity& entity) override;
};

class EnemyStunnedState : public TState<class EnemyStateMachine>
{
private:
	// Add private variables as needed

public:
	virtual ~EnemyStunnedState() {};
	void OnEnter(EnemyStateMachine* pOwner, Entity& entity) override;
	void OnExit(EnemyStateMachine* pOwner, Entity& entity) override;
	void Execute(EnemyStateMachine* pOwner, Entity& entity) override;
};