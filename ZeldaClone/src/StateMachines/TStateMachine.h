#pragma once
#include "TState.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"

template <class T>
class TStateMachine
{
private:
	T* mPOwner;
	TState<T>* mCurrentState;
	bool initialize = true;

public:
	TStateMachine()
	{
		//Logger::Log("Created State Machine");
	}

	~TStateMachine()
	{
		//Logger::Log("State Machine Destroyed");
	}

	void Init(T* pOwner, TState<T>* pInitialState);
	void Update(Entity& entity);
	void ChangeState(TState<T>* newState, Entity& entity);
};

// Template funciton initialization
template <class T>
void TStateMachine<T>::Init(T* pOwner, TState<T>* pInitialState)
{
	mCurrentState = pInitialState;
	mPOwner = pOwner;
	//Logger::Log("State machine is ready");
}

template <class T>
void TStateMachine<T>::Update(Entity& entity)
{
	if (initialize)
	{
		initialize = false;
		mCurrentState->OnEnter(mPOwner, entity);
	}
	mCurrentState->Execute(mPOwner, entity);
}

template <class T>
void TStateMachine<T>::ChangeState(TState<T>* newState, Entity& entity)
{
	mCurrentState->OnExit(mPOwner, entity);
	mCurrentState = newState;
	mCurrentState->OnEnter(mPOwner, entity);
}
