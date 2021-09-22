#pragma once

template <class T>
class TState
{
public:
	virtual ~TState() {}
	virtual void OnEnter(T* pOwner) = 0;
	virtual void OnExit(T* pOwner) = 0;
	virtual void Execute(T* pOwner) = 0;
};