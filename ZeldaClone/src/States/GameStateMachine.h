#pragma once

#include <vector>
#include <SDL.h>
#include "State.h"

class State;

class GameStateMachine
{
private:
	std::vector<State*> states;

public:
	GameStateMachine() {}
	~GameStateMachine() {}

	void Update(const float& deltaTime);
	void Render();

	void PushState(State* state);
	void ChangeState(State* state);
	void PopState();
	void ClearStates();

	std::string GetCurrentState();
	void ProcessEvents(SDL_Event& event);	
};