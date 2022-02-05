#pragma once
#include "NewState.h"

class Timer;
class Entity;

class EnemyIdleState : public States
{
public:
	virtual ~EnemyIdleState() {};
	void OnEnter(Entity& entity) override;
	void OnExit(Entity& entity) override;
	void Update(Entity& entity) override;
};

class EnemyAttackState : public States
{
public:
	virtual ~EnemyAttackState() {};
	void OnEnter(class Entity& entity) override;
	void OnExit (class Entity& entity) override;
	void Update (class Entity& entity) override;
};

class PatrolState : public States
{
public:
	virtual ~PatrolState() {};
	void OnEnter(class Entity& entity) override;
	void OnExit (class Entity& entity) override;
	void Update (class Entity& entity) override;
};

class HurtState : public States
{
private:
	// Add private variables as needed

public:
	virtual ~HurtState() {};
	void OnEnter(class Entity& entity) override;
	void OnExit (class Entity& entity) override;
	void Update(class Entity& entity) override;
};

class EnemyDeathState : public States
{
private:
	// Add private variables as needed

public:
	virtual ~EnemyDeathState() {};
	void OnEnter(class Entity& entity) override;
	void OnExit	(class Entity& entity) override;
	void Update (class Entity& entity) override;
};

class EnemyStunnedState : public States
{
private:
	// Add private variables as needed

public:
	virtual ~EnemyStunnedState() {};
	void OnEnter(class Entity& entity) override;
	void OnExit(class Entity& entity) override;
	void Update(class Entity& entity) override;
};
