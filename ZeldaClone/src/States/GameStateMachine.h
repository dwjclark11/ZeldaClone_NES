#pragma once

#include <vector>
#include <SDL.h>
#include "State.h"

class State;

class GameStateMachine
{
public:
	GameStateMachine() {}
	~GameStateMachine() {}

	void Update(const double& deltaTime);
	void Render();

	void PushState(State* state);
	void ChangeState(State* state);
	void PopState();
	void ClearStates();

	std::string GetCurrentState();

	void ProcessEvents(SDL_Event& event);
	void OnKeyDown(SDL_Event* event);
	void OnKeyUp(SDL_Event* event);
	void OnBtnDown(SDL_Event* event);
	void OnBtnUp(SDL_Event* event);
	
private:
	std::vector<State*> states;
};