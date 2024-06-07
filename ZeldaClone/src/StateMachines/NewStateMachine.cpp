#include "StateMachines/NewStateMachine.h"
#include "Logger/Logger.h"

StateMachine::StateMachine()
	: mNewState( nullptr )
	, mIsRemoving( false )
	, mIsAdding( false )
	, mIsReplacing( false )
{
	// Logger::Log("State Machine Created!");
}

StateMachine::~StateMachine()
{
	// Logger::Err("State Machine Destroyed!");
}

void StateMachine::AddState( StateRef newState, bool isReplacing )
{
	mIsAdding = true;
	mIsReplacing = isReplacing;

	mNewState = std::move( newState );
}

void StateMachine::RemoveState()
{
	mIsRemoving = true;
}

void StateMachine::ChangeState( Entity& entity )
{
	if ( mIsRemoving && !mStates.empty() )
	{
		mStates.pop();
		if ( !mStates.empty() )
		{
			mStates.top()->OnEnter( entity );
		}
		mIsRemoving = false;
	}

	if ( mIsAdding )
	{
		if ( !mStates.empty() )
		{
			if ( mIsReplacing )
			{
				mStates.top()->OnExit( entity );
				mStates.pop();
			}
			else
			{
				mStates.top()->OnExit( entity );
				mStates.pop();
			}
		}

		mStates.push( std::move( mNewState ) );
		mStates.top()->OnEnter( entity );
		mIsAdding = false;
	}
}

StateRef& StateMachine::GetCurrentState()
{
	return mStates.top();
}
