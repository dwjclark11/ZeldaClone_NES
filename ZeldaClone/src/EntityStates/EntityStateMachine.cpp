#include "EntityStateMachine.h"

void EntityStateMachine::Update(const double& deltaTime)
{
	if (!states.empty())
	{
		states.back()->Update(deltaTime);
	}
}

void EntityStateMachine::Render()
{
	if (!states.empty())
	{
		states.back()->Render();
	}
}


void EntityStateMachine::PushState(EntityState* state)
{
	// Once you push back the state, do the OnEnter function
	states.push_back(state);
	states.back()->OnEnter();
}

void EntityStateMachine::ChangeState(EntityState* state)
{
	if (!states.empty())
	{
		if (states.back()->GetStateId() == state->GetStateId())
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

void EntityStateMachine::PopState()
{
	if (!states.empty())
	{
		if (states.back()->OnExit())
		{
			states.erase(states.end() - 1);
		}
	}
}

void EntityStateMachine::ClearStates()
{
	if (!states.empty())
	{
		if (states.back()->OnExit())
		{
			states.erase(states.end() - 1);
			states.clear();
		}
	}
}
	
std::string EntityStateMachine::GetCurrentState()
{
	return states.back()->GetStateId();
}
	
void EntityStateMachine::ProcessEvents(SDL_Event& event)
{
	if (!states.empty())
	{
		states.back()->ProcessEvents(event);
	}
}