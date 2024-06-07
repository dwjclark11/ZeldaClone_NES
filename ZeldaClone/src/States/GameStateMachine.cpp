#include "States/GameStateMachine.h"
#include "States/State.h"

void GameStateMachine::Update( const float deltaTime )
{
	if ( !m_States.empty() )
	{
		m_States.back()->Update( deltaTime );
	}
}

void GameStateMachine::Render()
{
	if ( !m_States.empty() )
	{
		m_States.back()->Render();
	}
}

void GameStateMachine::PushState( std::unique_ptr<State> state, bool bReplace )
{
	if ( !m_States.empty() && bReplace )
	{
		m_States.back()->OnExit();
		m_States.erase( m_States.end() - 1 );
	}

	m_States.push_back( std::move( state ) );
	m_States.back()->OnEnter();
}

void GameStateMachine::PopState()
{
	if ( !m_States.empty() )
	{
		if ( m_States.back()->OnExit() )
		{
			m_States.erase( m_States.end() - 1 );
		}
	}
}

void GameStateMachine::ClearStates()
{
	if ( m_States.back()->OnExit() )
	{
		m_States.erase( m_States.end() - 1 );
		m_States.clear();
	}
}

std::string GameStateMachine::GetCurrentState()
{
	return m_States.back()->GetStateID();
}

void GameStateMachine::ProcessEvents( SDL_Event& event )
{
	if ( !m_States.empty() )
	{
		m_States.back()->ProcessEvents( event );
	}
}
