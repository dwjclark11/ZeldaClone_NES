#pragma once
#include "TState.h"
#include "../Logger/Logger.h"

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
		Logger::Log("Created State Machine");
	}

	~TStateMachine()
	{
		Logger::Log("State Machine Destroyed");
	}

	void Init(T* pOwner, TState<T>* pInitialState);
	void Update();
	void ChangeState(TState<T>* newState);
};

// Template funciton initialization
template <class T>
inline void TStateMachine<T>::Init(T* pOwner, TState<T>* pInitialState)
{
	mCurrentState = pInitialState;
	mPOwner = pOwner;
	Logger::Log("State machine is ready");
}

template <class T>
inline void TStateMachine<T>::Update()
{
	if (initialize)
	{
		initialize = false;
		mCurrentState->OnEnter(mPOwner);
	}
	mCurrentState->Execute(mPOwner);
}

template <class T>
inline void TStateMachine<T>::ChangeState(TState<T>* newState)
{
	mCurrentState->OnExit(mPOwner);
	mCurrentState = newState;
	mCurrentState->OnEnter(mPOwner);
}
