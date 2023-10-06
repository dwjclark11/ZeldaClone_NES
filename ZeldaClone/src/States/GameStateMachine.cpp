#include "GameStateMachine.h"

void GameStateMachine::Update(const float deltaTime)
{
	if (!states.empty())
	{
		states.back()->Update(deltaTime);
	}
}

void GameStateMachine::Render()
{
	if (!states.empty())
	{
		states.back()->Render();
	}
}

void GameStateMachine::PushState(std::unique_ptr<State> state, bool bReplace)
{
	if (!states.empty() && bReplace)
	{
		states.back()->OnExit();
		states.erase(states.end() - 1);
	}

	states.push_back(std::move(state));
	states.back()->OnEnter();
}

void GameStateMachine::PopState()
{
	if (!states.empty())
	{
		if (states.back()->OnExit())
		{
			states.erase(states.end() - 1);
		}
	}
}

void GameStateMachine::ClearStates()
{
	if (states.back()->OnExit())
	{
		states.erase(states.end() - 1);
		states.clear();
	}
}

std::string GameStateMachine::GetCurrentState()
{
	return states.back()->GetStateID();
}

void GameStateMachine::ProcessEvents(SDL_Event& event)
{
	if (!states.empty())
	{
		states.back()->ProcessEvents(event);
	}
}
