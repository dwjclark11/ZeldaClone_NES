#pragma once

class States
{
  public:
	virtual ~States() {}
	virtual void OnEnter( class Entity& entity ) = 0;
	virtual void Update( class Entity& entity ) = 0;
	virtual void OnExit( class Entity& entity ) = 0;
};