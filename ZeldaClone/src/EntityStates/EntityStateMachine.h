#pragma once
#include <vector>
#include "EntityStates.h"

class SDL_Event;

class EntityStateMachine
{
public:
	EntityStateMachine() {}
	~EntityStateMachine() {}

	void Update(const double& deltaTime);
	void Render();

	void PushState(EntityState* state);
	void ChangeState(EntityState* state);
	void PopState();
	void ClearStates();

	std::string GetCurrentState();

	void ProcessEvents(SDL_Event& event);

private:
	std::vector<EntityState*> states;
};