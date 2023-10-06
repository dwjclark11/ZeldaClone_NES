#pragma once

#include <vector>
#include <SDL.h>
#include "State.h"
#include <memory>
class State;

class GameStateMachine
{
private:
	std::vector<std::unique_ptr<State>> states;

public:
	GameStateMachine() {}
	~GameStateMachine() {}

	void Update(const float deltaTime);
	void Render();

	void PushState(std::unique_ptr<State> state, bool bReplace = true);
	void PopState();
	void ClearStates();

	std::string GetCurrentState();
	void ProcessEvents(SDL_Event& event);	
};