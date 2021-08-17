// EntityStates.h
#pragma once
#include <string>
#include "../Logger/Logger.h"
#include "EntityStateMachine.h"

class SDL_Event;

class EntityState
{
public:
	virtual void Update(const double& deltaTime) = 0;
	virtual void Render() = 0;

	virtual bool OnEnter() = 0;
	virtual bool OnExit() = 0;

	virtual void ProcessEvents(SDL_Event& event) = 0;

	virtual std::string GetStateId() const = 0;
};