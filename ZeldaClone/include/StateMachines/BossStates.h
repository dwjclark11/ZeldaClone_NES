#pragma once
#include "NewState.h"
#include "Utilities/Timer.h"
#include <glm/glm.hpp>

class Entity;

class BossIdleState : public States
{
  public:
	virtual ~BossIdleState(){};
	void OnEnter( Entity& entity ) override;
	void OnExit( Entity& entity ) override;
	void Update( Entity& entity ) override;
};

class BossAttackState : public States
{
  public:
	virtual ~BossAttackState(){};
	void OnEnter( class Entity& entity ) override;
	void OnExit( class Entity& entity ) override;
	void Update( class Entity& entity ) override;
};

class BossPatrolState : public States
{
  private:
	Timer walkTimer;
	glm::vec2 startPos = glm::vec2( 4672, 1600 );

  public:
	virtual ~BossPatrolState(){};
	void OnEnter( class Entity& entity ) override;
	void OnExit( class Entity& entity ) override;
	void Update( class Entity& entity ) override;
};

class BossHurtState : public States
{
  private:
	// Add private variables as needed

  public:
	virtual ~BossHurtState(){};
	void OnEnter( class Entity& entity ) override;
	void OnExit( class Entity& entity ) override;
	void Update( class Entity& entity ) override;
};

class BossDeathState : public States
{
  private:
	// Add private variables as needed

  public:
	virtual ~BossDeathState(){};
	void OnEnter( class Entity& entity ) override;
	void OnExit( class Entity& entity ) override;
	void Update( class Entity& entity ) override;

	void DropHeart( class Entity& entity );
};

class BossStunnedState : public States
{
  private:
	// Add private variables as needed

  public:
	virtual ~BossStunnedState(){};
	void OnEnter( class Entity& entity ) override;
	void OnExit( class Entity& entity ) override;
	void Update( class Entity& entity ) override;
};
