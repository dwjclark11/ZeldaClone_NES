#pragma once
#include "StateMachines/NewState.h"
#include <memory>
#include <stack>

typedef std::unique_ptr<States> StateRef;

class StateMachine
{
  private:
	std::stack<StateRef> mStates;
	StateRef mNewState;
	bool mIsRemoving;
	bool mIsAdding;
	bool mIsReplacing;

  public:
	StateMachine();
	~StateMachine();

	void AddState( StateRef newState, bool isReplacing = true );
	void RemoveState();
	void ChangeState( class Entity& entity );

	StateRef& GetCurrentState();
};