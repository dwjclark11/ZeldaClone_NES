#include "GameStateMachine.h"

void GameStateMachine::Update(const double& deltaTime)
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

void GameStateMachine::PushState(State* state)
{
	states.push_back(state);
	states.back()->OnEnter();
}

void GameStateMachine::ChangeState(State* state)
{
	if (!states.empty())
	{
		if (states.back()->GetStateID() == state->GetStateID())
		{
			return;
		}
	}
	states.push_back(state);

	if (!states.empty())
	{
		if (states.back()->OnExit())
		{
			states.erase(states.end() - 2);
		}
	}
	states.back()->OnEnter();
}

void GameStateMachine::PopState()
{
	if (!states.empty())
	{
		if (states.back()->OnExit())
		{
			//delete states.back();
			//states.back() = nullptr;
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

void GameStateMachine::OnKeyDown(SDL_Event* event)
{
	if (!states.empty())
	{
		states.back()->OnKeyDown(event);
	}
}

void GameStateMachine::OnKeyUp(SDL_Event* event)
{
	if (!states.empty())
	{
		states.back()->OnKeyUp(event);
	}
}

