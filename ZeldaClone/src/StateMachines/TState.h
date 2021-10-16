#pragma once
#include "../ECS/ECS.h"

template <class T>
class TState
{
public:
	virtual ~TState() {}
	virtual void OnEnter(T* pOwner, Entity& entity) = 0;
	virtual void OnExit(T* pOwner, Entity& entity) = 0;
	virtual void Execute(T* pOwner, Entity& entity) = 0;
};