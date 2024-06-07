#pragma once
#include "StateMachines/NewState.h"

class Timer;
class Entity;

class IdleState : public States
{
  public:
	void OnEnter( Entity& entity ) override;
	void OnExit( Entity& entity ) override;
	void Update( Entity& entity ) override;
};

class AttackState : public States
{
  public:
	void OnEnter( Entity& entity ) override;
	void OnExit( Entity& entity ) override;
	void Update( Entity& entity ) override;
};

class MoveState : public States
{
  public:
	void OnEnter( Entity& entity ) override;
	void OnExit( Entity& entity ) override;
	void Update( Entity& entity ) override;
};

class CollectItemState : public States
{
  private:
	// bool itemCollected = false;
	// bool movedTrigItem = false;

  public:
	void OnEnter( Entity& entity ) override;
	void OnExit( Entity& entity ) override;
	void Update( Entity& entity ) override;
};

class PlayerHurtState : public States
{
  private:
  public:
	void OnEnter( Entity& entity ) override;
	void OnExit( Entity& entity ) override;
	void Update( Entity& entity ) override;
};

class PlayerDeathState : public States
{
  private:
  public:
	void OnEnter( Entity& entity ) override;
	void OnExit( Entity& entity ) override;
	void Update( Entity& entity ) override;
};

class PlayerStairsState : public States
{
  private:
	int steps = 0;

  public:
	void OnEnter( Entity& entity ) override;
	void OnExit( Entity& entity ) override;
	void Update( Entity& entity ) override;
};